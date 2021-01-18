/**************************************************************
 *
 * --== Simple CUDA kernel ==--
 * author: ampereira
 *
 *
 * Fill the rest of the code
 *
 * Insert the functions for time measurement in the correct
 * sections (i.e. do not account for filling the vectors with random data)
 *
 *
 * The dot array size must be set to the SIZE #define, i.e., float dot[SIZE];
 **************************************************************/
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sys/time.h>

#define TIME_RESOLUTION 1000000	// time measuring resolution (us)

#define NUM_BLOCKS 128
#define NUM_THREADS_PER_BLOCK 128
#define SIZE NUM_BLOCKS*NUM_THREADS_PER_BLOCK
#define NEIGHBOURS 4

using namespace std;

long long unsigned cpu_time;
cudaEvent_t start, stop;
timeval t;

void startTime (void) {
    gettimeofday(&t, NULL);
    cpu_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

void stopTime (void) {
    gettimeofday(&t, NULL);
    long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

    final_time -= cpu_time;

    cout << final_time << " us have elapsed for the CPU execution" << endl;
}

// These are specific to measure the execution of only the kernel execution - might be useful
void startKernelTime (void) {
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);
}

void stopKernelTime (void) {
    cudaEventRecord(stop);

    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cout << milliseconds << " ms have elapsed for the CUDA execution" << endl;
}

void checkCUDAError (const char *msg) {
    cudaError_t err = cudaGetLastError();
    if( cudaSuccess != err) {
        cerr << "Cuda error: " << msg << ", " << cudaGetErrorString( err) << endl;
        exit(-1);
    }
}

// Fill the input parameters and kernel qualifier
__global__
void dotKernel (float *dev_m1, float *dev_m2, float *dev_output) {
    int id = blockIdx.x*blockDim.x+threadIdx.x;

    //block = blockId * blockDim - 0 -> max threadId
    int i = SIZE / id;
    int k = SIZE % id;
    for (size_t j = 0; k < NUM_THREADS_PER_BLOCK; k++) {
        dev_output[blockIdx.x*blockDim.x+j] += dev_m1[id] * dev_m2[k*blockDim.x+j];
    }
}

// Fill with the code required for the GPU dot (mem allocation, transfers, kernel launch of dotKernel)
float* dotGPU (float *m1, float *m2) {
    float *dev_m1, *dev_m2, *dev_output;
    float *array_output = new float [SIZE];

    // allocate the memory on the device
    cudaMalloc((void**) &dev_m1, sizeof(float) * SIZE);
    cudaMalloc((void**) &dev_m2, sizeof(float) * SIZE);
    cudaMalloc((void**) &dev_output, sizeof(float) * SIZE);

    startKernelTime();
    // copy inputs to the device
    cudaMemcpy(dev_m1, m1, sizeof(float) * SIZE, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_m2, m2, sizeof(float) * SIZE, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_output, array_output, sizeof(float) * SIZE, cudaMemcpyHostToDevice);

    // launch the kernel
    dotKernel <<< NUM_THREADS_PER_BLOCK, NUM_BLOCKS >>> (dev_m1, dev_m2, dev_output);

    // copy the output to the host
    cudaMemcpy(array_output, dev_output, sizeof(float) * SIZE, cudaMemcpyDeviceToHost);
    stopKernelTime();
    for(size_t i = 0; i < SIZE; i++)
        cout << array_output[i] << '\n';

    // free the device memory
    cudaFree(dev_m1);
    cudaFree(dev_m2);
    cudaFree(dev_output);

    return array_output;
}

int main (int argc, char** argv) {
    float array1 [SIZE];
    float array2 [SIZE];
    // initialize array with random values
    for (unsigned i = 0; i < SIZE; i++) {
        array1[i] = ((float) rand()) / ((float) RAND_MAX);
        array2[i] = ((float) rand()) / ((float) RAND_MAX);
    }
    dotGPU(array1, array2);
    return 0;
}

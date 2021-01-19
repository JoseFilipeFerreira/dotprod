#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#define BS 32
#define NUM_BLOCKS 1500
#define NUM_THREADS_PER_BLOCK 1500
#define SIZE NUM_BLOCKS*NUM_THREADS_PER_BLOCK

using namespace std;

cudaEvent_t start, stop;

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
    __shared__ float shareA[BS][BS];
    __shared__ float shareB[BS][BS];
    int bx = blockIdx.x; int by = blockIdx.y;
    int tx = threadIdx.x; int ty = threadIdx.y;
    int row = by * BS + ty;
    int col = bx * BS + tx;
    float tmp_sum = 0.0f;
    for(int i = 0; i < NUM_BLOCKS/BS; ++i){
        shareA[ty][tx] = dev_m1[row* NUM_BLOCKS + (i * BS + tx)];
        shareB[ty][tx] = dev_m2[(i * BS + ty) * NUM_BLOCKS + col];
        __syncthreads();
        for(int k = 0; k < BS; ++k){
            tmp_sum += shareA[ty][k] * shareB[k][tx];
            __syncthreads();
        }
        dev_output[row * NUM_BLOCKS + col] = tmp_sum;
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

    dim3 blocksPerGrid(BS, BS, 1);
    dim3 threadsPerBlock(NUM_THREADS_PER_BLOCK/BS, NUM_THREADS_PER_BLOCK/BS, 1);
    // launch the kernel
    dotKernel <<< blocksPerGrid, threadsPerBlock >>> (dev_m1, dev_m2, dev_output);

    // copy the output to the host
    cudaMemcpy(array_output, dev_output, sizeof(float) * SIZE, cudaMemcpyDeviceToHost);
    stopKernelTime();
    
    for(size_t i = 0; i < 512; i++) {
        cout << array_output[i] << '\n';
    }
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
        array1[i] = ((float) rand()) / ((float) RAND_MAX) * 10;
        array2[i] = 1;
    }
    for(size_t i = 0; i < 4; i++) {
        cout << array1[i * NUM_BLOCKS] << '\n';
    }
    dotGPU(array1, array2);
    return 0;
}

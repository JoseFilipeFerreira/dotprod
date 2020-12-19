#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <papi.h>

#define NUM_EVENTS 2
long long values[NUM_EVENTS];
unsigned int Events[NUM_EVENTS]={PAPI_TOT_INS,PAPI_TOT_CYC};

#define SIZE 128
float A[SIZE][SIZE];
float B[SIZE][SIZE];
float C[SIZE][SIZE];


void dotprod_seq(size_t size, float m1[][size], float m2[][size], float result[][size]){
    for(size_t i = 0; i < size; i++){
        for(size_t j = 0; j < size; j++){
            result[i][j]=0;
            for(size_t k = 0; k < size ; k++){
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

void create_matrices () {
    time_t t;

    srand((unsigned) time(&t) + clock());

    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            A[i][j] = ((float) rand()) / ((float) RAND_MAX);
            B[i][j] = 1;
        }
    }
}

int main(void){
    create_matrices();

    PAPI_start_counters((int*)Events,NUM_EVENTS);

    dotprod_seq(SIZE, A, B, C);

    retval = PAPI_stop_counters(values,NUM_EVENTS);

    printf("%f\n", C[0][0]);

    return 0;
}

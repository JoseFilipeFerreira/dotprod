#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/*
#include <papi.h>

#define NUM_EVENTS 2
long long values[NUM_EVENTS];
unsigned int Events[NUM_EVENTS]={PAPI_TOT_INS,PAPI_TOT_CYC};
*/

#define SIZE 100
#define MAX_BLOCK_SIDE 49
float A[SIZE][SIZE];
float B[SIZE][SIZE];
float C[SIZE][SIZE];

// order IJK
void dotprod_ijk(size_t size, float a[][size], float b[][size], float
        c[][size]){
    memset(c, 0, size * size * sizeof(float));

    for(size_t i = 0; i < size; i++)
        for(size_t j = 0; j < size; j++)
            for(size_t k = 0; k < size ; k++)
                c[i][j] += a[i][k] * b[k][j];
}

void dotprod_ijk_tranposed(size_t size, float a[][size], float b[][size], float
        c[][size]){
    memset(c, 0, size * size * sizeof(float));

    transpose_matrix(size, b);

    for(size_t i = 0; i < size; i++)
        for(size_t j = 0; j < size; j++)
            for(size_t k = 0; k < size ; k++)
                c[i][j] += a[i][k] * b[j][k];
}

// order IKJ
void dotprod_ikj(size_t size, float a[][size], float b[][size], float
        c[][size]){
    memset(c, 0, size * size * sizeof(float));

    for(size_t i = 0; i < size; i++)
        for(size_t k = 0; k < size ; k++)
            for(size_t j = 0; j < size; j++)
                c[i][j] += a[i][k] * b[k][j];
}

// order JKI
void dotprod_jki(size_t size, float a[][size], float b[][size], float
        c[][size]){
    memset(c, 0, size * size * sizeof(float));

    for(size_t j = 0; j < size; j++)
        for(size_t k = 0; k < size ; k++)
            for(size_t i = 0; i < size; i++)
                c[i][j] += a[i][k] * b[k][j];
}

void dotprod_jki_transposed(size_t size, float a[][size], float b[][size], float
        c[][size]){
    memset(c, 0, size * size * sizeof(float));

    transpose_matrix(size, a);
    transpose_matrix(size, b);

    for(size_t j = 0; j < size; j++)
        for(size_t k = 0; k < size ; k++)
            for(size_t i = 0; i < size; i++)
                c[j][i] += a[k][i] * b[j][k];

    transpose_matrix(size, c);
}

// block optimization
void dotprod_ikj_block(size_t size, float a[][size], float b[][size], float c[][size]){

    memset(c, 0, size * size * sizeof(float));

    size_t c_block_size = SIZE > MAX_BLOCK_SIDE ? SIZE / (SIZE / MAX_BLOCK_SIDE) : SIZE;

    printf("Size of Block: %zu\n", c_block_size);

    for(size_t bk = 0; bk < size; bk += c_block_size)
        for(size_t bj = 0; bj < size; bj += c_block_size)
            for(size_t i = 0; i < size; i++)
                for(size_t k = bk; k < bk + c_block_size; k++)
                    for(size_t j = bj; j < bj + c_block_size; j++)
                        c[i][j] += a[i][k] * b[k][j];

}

int main(void){
    matrices_rand_ones(SIZE, A, B);

    print_matrix(SIZE,A);
    putchar('\n');
    print_matrix(SIZE,B);
    putchar('\n');

    dotprod_ikj_block(SIZE, A, B, C);
    assert_result_lines(SIZE, C);
    print_matrix(SIZE, C);
    putchar('\n');

    dotprod_ikj_block(SIZE, B, A, C);
    assert_result_collums(SIZE, C);
    print_matrix(SIZE, C);
    putchar('\n');

    return 0;
}

#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void matrices_rand_ones (size_t size, float rands[size][size], float ones[size][size]) {
    time_t t;

    srand((unsigned) time(&t) + clock());

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            rands[i][j] = ((float) rand()) / ((float) RAND_MAX);
            ones[i][j] = 1;
        }
    }
}


void transpose_matrix(size_t size, float m[size][size]){
    float swap;
    for (size_t i = 0; i < size; i++){
        for(size_t j = 0; j < i; j++){
            swap = m[i][j];
            m[i][j] = m[j][i];
            m[j][i] = swap;
        }
    }
}

void assert_result_lines(size_t size, float m[size][size]){
    for(size_t i = 0; i < size; i++){
        for(size_t j = 0; j < size; j++){
            if (m[i][j] != m[i][0]){
                fprintf(stderr, "Error: Not all lines had the same values\n");
                exit(0);
            }
        }
    }
}

void assert_result_collums(size_t size, float m[size][size]){
    for(size_t j = 0; j < size; j++){
        for(size_t i = 0; i < size; i++){
            if(m[i][j] != m[0][j]){
                fprintf(stderr, "Error: Not all collums had the same values\n");
                exit(0);
            }
        }
    }
}

void print_matrix(size_t size, float m[size][size]){
    size_t max_print = 9;
    if (size <= max_print){
        for (size_t i = 0; i < size; i++){
            for(size_t j = 0; j < size; j++){
                printf("%.1f\t", m[i][j]);
            }
            putchar('\n');
        }

    } else {
        for (size_t i = 0; i < max_print - 2; i++){
            for(size_t j = 0; j < max_print - 2; j++){
                printf("%.1f\t", m[i][j]);
            }
            printf("...\t%.1f\n", m[i][size - 1]);
        }

        for(size_t j = 0; j < max_print; j++){
            printf("...\t");
        }
        putchar('\n');

        for(size_t j = 0; j < max_print - 2; j++){
            printf("%.1f\t", m[size - 1][j]);
        }
        printf("...\t%.1f\n", m[size - 1][size - 1]);

    }
}

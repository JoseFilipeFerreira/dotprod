#include "papi.h"
#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_EVENTS 19
int EVENTS[NUM_EVENTS] = {
    PAPI_TOT_INS,
    PAPI_TOT_CYC,
    PAPI_REF_CYC,
    PAPI_BR_CN,
    PAPI_BR_MSP,
    PAPI_L1_DCM,
    PAPI_L1_ICM,
    PAPI_L1_TCM,
    PAPI_L2_DCM,
    PAPI_L2_ICM,
    PAPI_L2_TCA,
    PAPI_L2_TCM,
    PAPI_L3_DCA,
    PAPI_L3_ICA,
    PAPI_L3_TCA,
    PAPI_L3_TCM,
    PAPI_FP_OPS,
    PAPI_VEC_SP,
    PAPI_VEC_DP};
int EVENTSET = PAPI_NULL;
long long VALUES[NUM_EVENTS];

#define SIZE 100
#define MAX_BLOCK_SIDE 49
float A[SIZE][SIZE];
float B[SIZE][SIZE];
float C[SIZE][SIZE];

// order IJK
void dotprod_ijk(
    size_t size, float a[][size], float b[][size], float c[][size]) {
    memset(c, 0, size * size * sizeof(float));

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            for (size_t k = 0; k < size; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void dotprod_ijk_tranposed(
    size_t size, float a[][size], float b[][size], float c[][size]) {
    memset(c, 0, size * size * sizeof(float));

    transpose_matrix(size, b);

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            for (size_t k = 0; k < size; k++) {
                c[i][j] += a[i][k] * b[j][k];
            }
        }
    }
}

// order IKJ
void dotprod_ikj(
    size_t size, float a[][size], float b[][size], float c[][size]) {
    memset(c, 0, size * size * sizeof(float));

    for (size_t i = 0; i < size; i++) {
        for (size_t k = 0; k < size; k++) {
            for (size_t j = 0; j < size; j++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// order JKI
void dotprod_jki(
    size_t size, float a[][size], float b[][size], float c[][size]) {
    memset(c, 0, size * size * sizeof(float));

    for (size_t j = 0; j < size; j++) {
        for (size_t k = 0; k < size; k++) {
            for (size_t i = 0; i < size; i++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}
void dotprod_jki_transposed(
    size_t size, float a[][size], float b[][size], float c[][size]) {
    memset(c, 0, size * size * sizeof(float));

    transpose_matrix(size, a);
    transpose_matrix(size, b);

    for (size_t j = 0; j < size; j++) {
        for (size_t k = 0; k < size; k++) {
            for (size_t i = 0; i < size; i++) {
                c[j][i] += a[k][i] * b[j][k];
            }
        }
    }

    transpose_matrix(size, c);
}

// block optimization
void dotprod_ikj_block(
    size_t size, float a[][size], float b[][size], float c[][size]) {

    memset(c, 0, size * size * sizeof(float));

    size_t c_block_size =
        SIZE > MAX_BLOCK_SIDE ? SIZE / (SIZE / MAX_BLOCK_SIDE) : SIZE;

    for (size_t bk = 0; bk < size; bk += c_block_size) {
        for (size_t bj = 0; bj < size; bj += c_block_size) {
            for (size_t i = 0; i < size; i++) {
                for (size_t k = bk; k < bk + c_block_size; k++) {
                    for (size_t j = bj; j < bj + c_block_size; j++) {
                        c[i][j] += a[i][k] * b[k][j];
                    }
                }
            }
        }
    }
}

void print_CSV_header(){
    printf("name");

    char* event_name = malloc(sizeof(char) * PAPI_MAX_STR_LEN);
    for (size_t i = 0; i < NUM_EVENTS; i++) {
        PAPI_event_code_to_name(EVENTS[i], event_name);
        printf(",%s", event_name);
    }
    putchar('\n');

    free(event_name);
}

void print_PAPI_result_CSV() {
    for (size_t i = 0; i < NUM_EVENTS; i++) {
        printf(",%lld", VALUES[i]);
    }
    putchar('\n');
}

int main(void) {
    // Initialize PAPI
    PAPI_library_init(PAPI_VER_CURRENT);
    PAPI_create_eventset(&EVENTSET);
    PAPI_add_events(EVENTSET, EVENTS, NUM_EVENTS);

    matrices_rand_ones(SIZE, A, B);

    print_CSV_header();

    printf("Dotprod ijk");

    PAPI_start(EVENTSET);
    dotprod_ijk(SIZE, A, B, C);
    PAPI_stop(EVENTSET, VALUES);

    assert_result_lines(SIZE, C);
    dotprod_ijk(SIZE, B, A, C);
    assert_result_collums(SIZE, C);

    print_PAPI_result_CSV();
    PAPI_reset(EVENTSET);

    printf("Dotprod ijk transposed");

    PAPI_start(EVENTSET);
    dotprod_ijk_tranposed(SIZE, A, B, C);
    PAPI_stop(EVENTSET, VALUES);

    assert_result_lines(SIZE, C);
    dotprod_ijk_tranposed(SIZE, B, A, C);
    assert_result_collums(SIZE, C);

    print_PAPI_result_CSV();
    PAPI_reset(EVENTSET);

    printf("Dotprod ikj");

    PAPI_start(EVENTSET);
    dotprod_ikj(SIZE, A, B, C);
    PAPI_stop(EVENTSET, VALUES);

    assert_result_lines(SIZE, C);
    dotprod_ikj(SIZE, B, A, C);
    assert_result_collums(SIZE, C);

    print_PAPI_result_CSV();
    PAPI_reset(EVENTSET);

    printf("Dotprod jki");

    PAPI_start(EVENTSET);
    dotprod_jki(SIZE, A, B, C);
    PAPI_stop(EVENTSET, VALUES);

    assert_result_lines(SIZE, C);
    dotprod_jki(SIZE, B, A, C);
    assert_result_collums(SIZE, C);

    print_PAPI_result_CSV();
    PAPI_reset(EVENTSET);

    printf("Dotprod jki transposed");

    PAPI_start(EVENTSET);
    dotprod_jki_transposed(SIZE, A, B, C);
    PAPI_stop(EVENTSET, VALUES);

    assert_result_lines(SIZE, C);
    dotprod_jki_transposed(SIZE, B, A, C);
    assert_result_collums(SIZE, C);

    print_PAPI_result_CSV();
    PAPI_reset(EVENTSET);

    printf("Dotprod ikj w/ block optimization");

    PAPI_start(EVENTSET);
    dotprod_ikj_block(SIZE, A, B, C);
    PAPI_stop(EVENTSET, VALUES);

    assert_result_lines(SIZE, C);
    dotprod_ikj_block(SIZE, B, A, C);
    assert_result_collums(SIZE, C);

    print_PAPI_result_CSV();
    PAPI_reset(EVENTSET);

    return 0;
}

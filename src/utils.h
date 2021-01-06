#ifndef ___UTILS_H___
#define ___UTILS_H___

#include <stdlib.h>

void matrices_rand_ones (size_t size, float rands[size][size], float ones[size][size]);

void transpose_matrix(size_t size, float m[size][size]);

void assert_result_lines(size_t size, float m[size][size]);

void assert_result_collums(size_t size, float m[size][size]);

void print_matrix(size_t size, float m[size][size]);

#endif

#ifndef NEURC_MATRIX_H

#define NEURC_MATRIX_H

#include <stddef.h>

typedef double m_double;

typedef struct {
    m_double *data; //  col_size * i + j
    size_t row_size;
    size_t col_size;
} Matrix;


typedef enum {
    NEURC_ALLOC_OK,
    NEURC_ALLOC_ERROR,
    NEURC_INCORRECT_SHAPE,
    NEURC_INCORRECT_ARGS
} ErrorTypes;


Matrix *create_matrix(size_t rows, size_t cols);
void destroy_matrix(Matrix *matrix);
void print_error(ErrorTypes err_index);
void print_matrix(Matrix *m);

#endif
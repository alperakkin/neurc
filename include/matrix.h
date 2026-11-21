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
    NEURC_OK,
    NEURC_ALLOC_ERROR,
    NEURC_INCORRECT_SHAPE,
    NEURC_INCORRECT_ARGS
} ErrorTypes;


Matrix *mat_create(size_t rows, size_t cols);
void mat_destroy(Matrix *matrix);
void panic(ErrorTypes err_index);
void mat_print(const Matrix *m);
m_double get_element(const Matrix *m, size_t row, size_t col);
void set_element(m_double value, Matrix *m, size_t row, size_t col);
void mat_add(Matrix *a, const Matrix *b);
void mat_sub(Matrix *a, const Matrix *b);
void mat_add_scalar(Matrix *a,  m_double s);
void mat_sub_scalar(Matrix *a,  m_double s);
void matmul(const Matrix *a, const Matrix *b, Matrix *result);
void mat_x_scalar(Matrix *a,  m_double s);
void hadamard(Matrix *a, const Matrix *b);
Matrix *transpose(const Matrix *a);

#endif
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
ErrorTypes mat_destroy(Matrix *matrix);
void print_error(ErrorTypes err_index);
ErrorTypes mat_print(const Matrix *m);
m_double get_element(const Matrix *m, size_t row, size_t col);
ErrorTypes set_element(m_double value, Matrix *m, size_t row, size_t col);
ErrorTypes mat_add(Matrix *a, const Matrix *b);
ErrorTypes mat_sub(Matrix *a, const Matrix *b);
ErrorTypes mat_add_scalar(Matrix *a,  m_double s);
ErrorTypes mat_sub_scalar(Matrix *a,  m_double s);
ErrorTypes matmul(const Matrix *a, const Matrix *b, Matrix *result);
ErrorTypes mat_x_scalar(Matrix *a,  m_double s);
ErrorTypes hadamard(Matrix *a, const Matrix *b);
Matrix *transpose(const Matrix *a);

#endif
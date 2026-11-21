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
    NEURC_MATMUL_NOT_MATCHED,
    NEURC_INCORRECT_SIZE,
    NEURC_INCORRECT_ARGS
} ErrorTypes;


Matrix *mat_create(size_t rows, size_t cols, const m_double* data);

void mat_destroy(Matrix *matrix);
void mat_print(const Matrix *m);
void print_size(char *name, const Matrix *m);
void panic(const Matrix *a, const Matrix *b, ErrorTypes err_index);

m_double get_element(const Matrix *m, size_t row, size_t col);
void set_element(m_double value, Matrix *m, size_t row, size_t col);
size_t get_size(const Matrix *m);
void assert_matrix_size(const Matrix *m , size_t row, size_t col);
void assert_matrix_is_allocated(const Matrix *m);
void assert_matrixes_have_identical_shape(const Matrix *a, const Matrix *b);
void assert_matmul_shapes(const Matrix * a, const Matrix *b);

void mat_add(Matrix *a, const Matrix *b);
void mat_sub(Matrix *a, const Matrix *b);
void mat_add_scalar(Matrix *a,  m_double s);
void mat_sub_scalar(Matrix *a,  m_double s);
void mat_multiply_scalar(Matrix *a,  m_double s);
void hadamard(Matrix *a, const Matrix *b);
void matmul(const Matrix *a, const Matrix *b, Matrix *result);
void transpose(const Matrix *a, Matrix *result);
void map(Matrix *m, m_double (*func)(m_double));

#endif
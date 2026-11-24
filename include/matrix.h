#ifndef NEURC_MATRIX_H

#define NEURC_MATRIX_H


#include <stdint.h>


typedef double m_double;

typedef struct {
    m_double *data; //  col_size * i + j
    uint32_t row_size;
    uint32_t col_size;
} Matrix;




Matrix *mat_create(uint32_t rows, uint32_t cols, const m_double* data);

void mat_destroy(Matrix *matrix);
void mat_copy(const Matrix *src, Matrix *dest);

void print_size(char *name, const Matrix *m);
void fill(Matrix *m, double val);
void random_fill(Matrix *m, m_double min, m_double max);

m_double get_element(const Matrix *m, uint32_t row, uint32_t col);
void set_element(m_double value, Matrix *m, uint32_t row, uint32_t col);
uint32_t get_size(const Matrix *m);
void assert_matrix_size(const Matrix *m , uint32_t row, uint32_t col);
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
void add_bias(Matrix *m, Matrix *b);
void mat_slice_rows(const Matrix *src, Matrix *dest, uint32_t start_row, uint32_t row_count);
#endif
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"

#include "matrix.h"


void print_size(char* name, const Matrix *m)
{
    if(m == NULL) return;
    printf("%s Shape: (%u, %u)\n", name, m->row_size, m->col_size);
}
Matrix *mat_create(uint32_t rows, uint32_t cols,const m_double *data)
{

    if(rows==0 || cols==0) panic("Create Matrix", NEURC_INCORRECT_SHAPE); 
    if (rows * cols > UINT32_MAX) panic("Create Matrix", NEURC_INCORRECT_SIZE);
       
    Matrix *matrix = calloc(1, sizeof(Matrix));
    if (matrix == NULL) panic("Create Matrix:", NEURC_ALLOC_ERROR);
      
    
    matrix->col_size = cols;
    matrix->row_size = rows;
    matrix->data = calloc(cols*rows, sizeof(m_double));
    if (data != NULL) memcpy(matrix->data, data, sizeof(m_double)*get_size(matrix));
    return matrix;

}

void mat_copy(const Matrix *src, Matrix *dest)
{
    assert_matrix_is_allocated(src);
    if(dest == NULL) panic("Copy Matrix", NEURC_ALLOC_ERROR);
    memcpy(dest->data, src->data, sizeof(m_double)*src->col_size*src->row_size);
  

}

void fill(Matrix *m, double val)
{
    assert_matrix_is_allocated(m);

    for(uint32_t index=0; index < get_size(m); index++)
        m->data[index] = val;
    
}
void random_fill(Matrix *m, m_double min, m_double max)
{
    assert_matrix_is_allocated(m);
    for(uint32_t index=0; index < get_size(m); index++){

    

        m_double rand_val = (double)rand() / RAND_MAX;
        rand_val = min + (max - min) * rand_val;
       
        m->data[index] = rand_val;
    }
    

}

void mat_destroy(Matrix *m)
{
    if(m == NULL)  return;
    free(m->data);
    free(m);

}

void mat_print(const Matrix *m)
{
      if(m == NULL)  return;
  

      printf("Shape: (%ux%u)\n", m->row_size, m->col_size);
      printf("--------------------------------------------\n");
      printf("[\n");
      for(uint32_t row=0; row<m->row_size; row++){
        for(uint32_t col=0; col<m->col_size; col++){
            printf("\t%f", m->data[m->col_size * row + col]);
        }
        printf("\n");
      }
      printf("]\n");

}

void assert_matrix_is_allocated(const Matrix *m)
{
    if(m == NULL) panic("Assert Matrix Allocated", NEURC_ALLOC_ERROR);
}

void assert_matrixes_have_identical_shape(const Matrix *a, const Matrix *b)
{
    if (a->col_size != b->col_size || a->row_size != b->row_size) panic("Assert Matrix Has Identical Shape", NEURC_INCORRECT_SHAPE);
}

void assert_matmul_shapes(const Matrix * a, const Matrix *b)
{
    if (a->col_size != b->row_size) panic("Assert Matrix Matmul Shape", NEURC_MATMUL_NOT_MATCHED);
}

void assert_matrix_size(const Matrix *m , uint32_t row, uint32_t col)
{
    if (m->col_size != col || m->row_size != row) panic("Assert Matrix Size", NEURC_INCORRECT_SIZE);
}
uint32_t get_size(const Matrix *m)
{
    assert_matrix_is_allocated(m);

    return m->col_size * m->row_size;
}

m_double get_element(const Matrix *m, uint32_t row, uint32_t col)
{
    assert_matrix_is_allocated(m);
    if(row > m->row_size || col > m->col_size) panic("Get Matrix Element", NEURC_INCORRECT_SHAPE);


    return m->data[m->col_size * row + col];

}
void set_element(m_double value, Matrix *m, uint32_t row, uint32_t col)
{
    assert_matrix_is_allocated(m);
    if(row > m->row_size || col > m->col_size) panic("Set Matrix Element", NEURC_INCORRECT_SHAPE);
    m->data[row * m->col_size + col] = value;

}
void mat_add(Matrix *a, const Matrix *b)
{
    assert_matrix_is_allocated(a);
    assert_matrix_is_allocated(b);
    assert_matrixes_have_identical_shape(a, b);

    for(uint32_t index=0; index < get_size(a); index++)
    {
        a->data[index] += b->data[index];
    }

}


void mat_sub(Matrix *a, const Matrix *b)
{
    assert_matrix_is_allocated(a);
    assert_matrix_is_allocated(b);
    assert_matrixes_have_identical_shape(a, b);

    for(uint32_t index=0; index < get_size(a); index++)
    {
        a->data[index] -= b->data[index];
    }

}

void mat_add_scalar(Matrix *a,  m_double s)
{
    assert_matrix_is_allocated(a);
    for(uint32_t index=0; index < get_size(a); index++)
    {
        a->data[index] += s;
    }
}

void mat_sub_scalar(Matrix *a,  m_double s)
{
    assert_matrix_is_allocated(a);
    for(uint32_t index=0; index < get_size(a); index++)
    {
        a->data[index] -= s;
    }
}

void mat_multiply_scalar(Matrix *a,  m_double s)
{
    assert_matrix_is_allocated(a);
    for(uint32_t index=0; index < get_size(a); index++)
    {
        a->data[index] *= s;
    }
}

void hadamard(Matrix *a, const Matrix *b)
{
    assert_matrix_is_allocated(a);
    assert_matrix_is_allocated(b);
    assert_matrixes_have_identical_shape(a, b);
   
    for(uint32_t index=0; index < get_size(a); index++)
    {
        a->data[index] *= b->data[index];
    }
}

void matmul(const Matrix *a, const Matrix *b, Matrix *result)
{
    assert_matmul_shapes(a, b);
    assert_matrix_size(result, a->row_size, b->col_size);

    for (uint32_t row=0; row<result->row_size; row++)
    {
        for (uint32_t col=0; col<result->col_size; col++)
        {
            m_double dot_product = 0.0; 
            for (uint32_t k=0; k< a->col_size; k++)
            {
                dot_product += get_element(a, row, k) * get_element(b, k, col);
            }
            set_element(dot_product, result, row, col);

        }

    }

}

void transpose(const Matrix *a, Matrix *result)
{
    assert_matrix_is_allocated(a);
    assert_matrix_is_allocated(result);
    assert_matrix_size(a, result->col_size, result->row_size);

    for(uint32_t row=0; row<a->row_size; row++)
    {
        for(uint32_t col=0; col<a->col_size; col++)
        {
            
            set_element(get_element(a, row, col), result, col, row);
        }

    }

}

void map(Matrix *m, m_double (*func)(m_double))
{
    assert_matrix_is_allocated(m);
    for(uint32_t index=0; index<get_size(m); index++)
        m->data[index] = func(m->data[index]);
    
}

void add_bias(Matrix *m, Matrix *b)
{
    assert_matrix_is_allocated(m);
    assert_matrix_is_allocated(b);
    assert_matrix_size(b, 1, m->col_size);

    for(uint32_t row=0; row<m->row_size; row++)
    {
        for(uint32_t col=0; col<m->col_size; col++)
        {
            m_double biased = get_element(m, row, col) + get_element(b, 0, col);
           
            set_element(biased, m, row, col);
        }

    }
    


}
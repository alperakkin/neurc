#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

void panic(ErrorTypes err_index)
{
    fprintf(stderr, "\033[31mERROR: ");   
    switch (err_index)
    {
   
    case NEURC_INCORRECT_ARGS:
        fprintf(stderr, "Incorrect Matrix Arguments");
        break;
    case NEURC_INCORRECT_SHAPE:
        fprintf(stderr, "Incorrect Matrix Shape");
        break;
    
    default:
        fprintf(stderr, "Undefined Error");
        break;
    }
    fprintf(stderr, "\033[0m\n");
    exit(err_index);
}

Matrix *mat_create(size_t rows, size_t cols)
{

    if(rows==0 || cols==0)
    {
        panic(NEURC_INCORRECT_SHAPE);
        return NULL;
     
    }

    if (rows * cols > SIZE_MAX)
    {
        panic(NEURC_INCORRECT_SHAPE);
        return NULL;
      
    }
    Matrix *matrix = calloc(1, sizeof(Matrix));
    if (matrix == NULL){
        panic(NEURC_ALLOC_ERROR);
        return NULL;
    }
    matrix->col_size = cols;
    matrix->row_size = rows;
    matrix->data = calloc(cols*rows, sizeof(m_double));
    return matrix;

}

void mat_destroy(Matrix *m)
{
    if(m == NULL)  panic(NEURC_ALLOC_ERROR);
    free(m->data);
    free(m);

}

void mat_print(const Matrix *m)
{
      if(m == NULL)  panic(NEURC_ALLOC_ERROR);
  

      printf("Matrix has shape (%zux%zu)\n", m->row_size, m->col_size);
      printf("--------------------------------------------\n");
      printf("[\n");
      for(size_t row=0; row<m->row_size; row++){
        for(size_t col=0; col<m->col_size; col++){
            printf("\t%f", m->data[m->col_size * row + col]);
        }
        printf("\n");
      }
      printf("]\n");

}

void assert_matrix_is_allocated(const Matrix *m)
{
    if(m == NULL) panic(NEURC_ALLOC_ERROR);
}

void assert_matrixes_have_identical_shape(const Matrix *a, const Matrix *b)
{
    if (a->col_size != b->col_size || a->row_size != b->row_size) panic(NEURC_INCORRECT_SHAPE);
}

size_t get_size(const Matrix *m)
{
    assert_matrix_is_allocated(m);

    return m->col_size * m->row_size;
}

m_double get_element(const Matrix *m, size_t row, size_t col)
{
    assert_matrix_is_allocated(m);
    if(row > m->row_size || col > m->col_size) panic(NEURC_INCORRECT_SHAPE);


    return m->data[m->col_size * row + col];

}
void set_element(m_double value, Matrix *m, size_t row, size_t col)
{
    assert_matrix_is_allocated(m);
    if(row > m->row_size || col > m->col_size) panic(NEURC_INCORRECT_SHAPE);
    m->data[row * m->col_size + col] = value;

}
void mat_add(Matrix *a, const Matrix *b)
{
    assert_matrix_is_allocated(a);
    assert_matrix_is_allocated(b);
    assert_matrixes_have_identical_shape(a, b);

    for(size_t index=0; index < get_size(a); index++)
    {
        a->data[index] += b->data[index];
    }

}

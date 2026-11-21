#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

void panic(ErrorTypes err_index)
{
    switch (err_index)
    {
    case NEURC_INCORRECT_ARGS:
        fprintf(stderr, "Incorrent Matrix Arguments\n");
        break;
    case NEURC_INCORRECT_SHAPE:
        fprintf(stderr, "Incorrent Matrix Shape\n");
        break;
    
    default:
        break;
    }
    
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

m_double get_element(const Matrix *m, size_t row, size_t col)
{
    if(m == NULL) panic(NEURC_ALLOC_ERROR);
    if(row > m->row_size || col > m->col_size) panic(NEURC_INCORRECT_SHAPE);


    return m->data[m->col_size * row + col];

}
void set_element(m_double value, Matrix *m, size_t row, size_t col)
{
    if(m == NULL) panic(NEURC_ALLOC_ERROR);
    if(row > m->row_size || col > m->col_size) panic(NEURC_INCORRECT_SHAPE);
    m->data[row * m->col_size + col] = value;

}
void mat_add(Matrix *a, const Matrix *b);

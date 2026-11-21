#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

void print_error(ErrorTypes err_index)
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
    
}

Matrix *create_matrix(size_t rows, size_t cols)
{

    if(rows==0 || cols==0)
    {
        print_error(NEURC_INCORRECT_SHAPE);
        return NULL;
     
    }

    if (rows * cols > SIZE_MAX)
    {
        print_error(NEURC_INCORRECT_SHAPE);
        return NULL;
      
    }
    Matrix *matrix = calloc(1, sizeof(Matrix));
    if (matrix == NULL){
        print_error(NEURC_ALLOC_ERROR);
        return NULL;
    }
    matrix->col_size = cols;
    matrix->row_size = rows;
    matrix->data = calloc(cols*rows, sizeof(double));
    return matrix;

}

void destroy_matrix(Matrix *m)
{
    if(m == NULL) return;
    free(m->data);
    free(m);
}

void print_matrix(Matrix *m)
{
      if(m == NULL) {
        print_error(NEURC_ALLOC_ERROR);
        return;
      };

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

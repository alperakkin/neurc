#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "matrix.h"

void panic(const Matrix *a, const Matrix *b, ErrorTypes err_index)
{
    fprintf(stderr, "\033[31mERROR: \n");
    print_size("First Matrix - ", a);  
    print_size("Second Matrix - ", b);   
    switch (err_index)
    {
   
    case NEURC_INCORRECT_ARGS:
        fprintf(stderr, "Incorrect Matrix Arguments");
        break;
    case NEURC_INCORRECT_SHAPE:
        fprintf(stderr, "Incorrect Matrix Shape");
        break;
    
    case NEURC_INCORRECT_SIZE:
        fprintf(stderr, "Incorrect Matrix Size");
        break;
    case NEURC_MATMUL_NOT_MATCHED:
        fprintf(stderr, "Matrix columns not matched for MATMUL operations");
        break;   
    default:
        fprintf(stderr, "Undefined Error");
        break;
    }
    fprintf(stderr, "\033[0m\n");
    exit(err_index);
}

void print_size(char* name, const Matrix *m)
{
    if(m == NULL) return;
    printf("%s Shape: (%zu, %zu)\n", name, m->row_size, m->col_size);
}
Matrix *mat_create(size_t rows, size_t cols,const m_double *data)
{

    if(rows==0 || cols==0) panic(NULL,NULL, NEURC_INCORRECT_SHAPE); 
    if (rows * cols > SIZE_MAX) panic(NULL, NULL, NEURC_INCORRECT_SIZE);
       
    Matrix *matrix = calloc(1, sizeof(Matrix));
    if (matrix == NULL) panic(NULL,NULL, NEURC_ALLOC_ERROR);
      
    
    matrix->col_size = cols;
    matrix->row_size = rows;
    matrix->data = calloc(cols*rows, sizeof(m_double));
    if (data != NULL) memcpy(matrix->data, data, sizeof(m_double)*get_size(matrix));
    return matrix;

}

Matrix *mat_copy(Matrix *src)
{
    assert_matrix_is_allocated(src);
    Matrix * dest = mat_create(src->row_size, src->col_size, src->data);
    if(dest == NULL) panic(dest, NULL, NEURC_ALLOC_ERROR);
    return dest;

}

void fill(Matrix *m, double val)
{
    assert_matrix_is_allocated(m);

    for(size_t index=0; index < get_size(m); index++)
        m->data[index] -= val;
    
}
void random_fill(Matrix *m)
{

    assert_matrix_is_allocated(m);
    for (int i = 0; i < 10; i++) {
        rand();
    } // skipping initialization correlation


    for(size_t index=0; index < get_size(m); index++)
        m->data[index] = (double)rand()/RAND_MAX;
    

}

void mat_destroy(Matrix *m)
{
    if(m == NULL)  panic(m, NULL, NEURC_ALLOC_ERROR);
    free(m->data);
    free(m);

}

void mat_print(const Matrix *m)
{
      if(m == NULL)  return;
  

      printf("Shape: (%zux%zu)\n", m->row_size, m->col_size);
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
    if(m == NULL) panic(m, NULL, NEURC_ALLOC_ERROR);
}

void assert_matrixes_have_identical_shape(const Matrix *a, const Matrix *b)
{
    if (a->col_size != b->col_size || a->row_size != b->row_size) panic(a, b, NEURC_INCORRECT_SHAPE);
}

void assert_matmul_shapes(const Matrix * a, const Matrix *b)
{
    if (a->col_size != b->row_size) panic(a, b, NEURC_MATMUL_NOT_MATCHED);
}

void assert_matrix_size(const Matrix *m , size_t row, size_t col)
{
    if (m->col_size != col || m->row_size != row) panic(m, NULL, NEURC_INCORRECT_SIZE);
}
size_t get_size(const Matrix *m)
{
    assert_matrix_is_allocated(m);

    return m->col_size * m->row_size;
}

m_double get_element(const Matrix *m, size_t row, size_t col)
{
    assert_matrix_is_allocated(m);
    if(row > m->row_size || col > m->col_size) panic(m, NULL, NEURC_INCORRECT_SHAPE);


    return m->data[m->col_size * row + col];

}
void set_element(m_double value, Matrix *m, size_t row, size_t col)
{
    assert_matrix_is_allocated(m);
    if(row > m->row_size || col > m->col_size) panic(m, NULL, NEURC_INCORRECT_SHAPE);
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


void mat_sub(Matrix *a, const Matrix *b)
{
    assert_matrix_is_allocated(a);
    assert_matrix_is_allocated(b);
    assert_matrixes_have_identical_shape(a, b);

    for(size_t index=0; index < get_size(a); index++)
    {
        a->data[index] -= b->data[index];
    }

}

void mat_add_scalar(Matrix *a,  m_double s)
{
    assert_matrix_is_allocated(a);
    for(size_t index=0; index < get_size(a); index++)
    {
        a->data[index] += s;
    }
}

void mat_sub_scalar(Matrix *a,  m_double s)
{
    assert_matrix_is_allocated(a);
    for(size_t index=0; index < get_size(a); index++)
    {
        a->data[index] -= s;
    }
}

void mat_multiply_scalar(Matrix *a,  m_double s)
{
    assert_matrix_is_allocated(a);
    for(size_t index=0; index < get_size(a); index++)
    {
        a->data[index] *= s;
    }
}

void hadamard(Matrix *a, const Matrix *b)
{
    assert_matrix_is_allocated(a);
    assert_matrix_is_allocated(b);
    assert_matrixes_have_identical_shape(a, b);
   
    for(size_t index=0; index < get_size(a); index++)
    {
        a->data[index] *= b->data[index];
    }
}

void matmul(const Matrix *a, const Matrix *b, Matrix *result)
{
    assert_matmul_shapes(a, b);
    assert_matrix_size(result, a->row_size, b->col_size);

    for (size_t row=0; row<result->row_size; row++)
    {
        for (size_t col=0; col<result->col_size; col++)
        {
            m_double dot_product = 0.0; 
            for (size_t k=0; k< a->col_size; k++)
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

    for(size_t row=0; row<a->row_size; row++)
    {
        for(size_t col=0; col<a->col_size; col++)
        {
            
            set_element(get_element(a, row, col), result, col, row);
        }

    }

}

void map(Matrix *m, m_double (*func)(m_double))
{
    assert_matrix_is_allocated(m);
    for(size_t index=0; index<get_size(m); index++)
        m->data[index] = func(m->data[index]);
    
}

void add_bias(Matrix *m, Matrix *b)
{
    assert_matrix_is_allocated(m);
    assert_matrix_is_allocated(b);
    assert_matrix_size(b, 1, m->col_size);

    for(size_t row=0; row<m->row_size; row++)
    {
        for(size_t col=0; col<m->col_size; col++)
        {
            m_double biased = get_element(m, row, col) + get_element(b, 0, col);
           
            set_element(biased, m, row, col);
        }

    }
    


}
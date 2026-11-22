#include "loss.h"
#include <math.h>
#include <stdio.h>


m_double mean_square_error(Matrix *output, Matrix *target)
{
    assert_matrix_is_allocated(output);
    assert_matrix_is_allocated(target);


    m_double total_loss=0.0;
    for(size_t index=0; index < get_size(output);index++)
    {
        m_double dist = target->data[index] - output->data[index];
        total_loss +=(double) dist * dist;
    }
    return (double)total_loss/get_size(output);
}

void gradient_loss(Matrix *output, Matrix *target, Matrix *gradient)
{
    assert_matrix_is_allocated(output);
    assert_matrix_is_allocated(target);
    assert_matrix_is_allocated(gradient);
    assert_matrix_size(gradient, target->row_size, target->col_size);
    assert_matrix_size(output, target->row_size, target->col_size);
    m_double N = (m_double) get_size(target);
    for(size_t index=0; index < get_size(output);index++)
    {
        m_double dist =  output->data[index] - target->data[index];
        gradient->data[index] = (m_double) (2 / N) *dist;
    }

}

void print_loss(m_double loss)
{
    printf("------------------------------------------------------------\n");
    printf("\nLoss: %f\n", loss);
    printf("------------------------------------------------------------\n");
}
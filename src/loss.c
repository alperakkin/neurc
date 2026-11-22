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

void print_loss(m_double loss)
{
    printf("------------------------------------------------------------\n");
    printf("\nLoss: %f\n", loss);
    printf("------------------------------------------------------------\n");
}
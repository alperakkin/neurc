#include <math.h>
#include <stddef.h>
#include "activation.h"


double sigmoid_func(double val)
{
    // 1 / (1 + e pow(-x))
    return (double) 1/(1 + exp(-val));
}
double relu_func(double val)
{
    if (val<0) return 0;
    return val;
}

double tanh_func(double val)
{
    // (exp(x) - exp(-x)) / (exp(x) + exp(-x))
    return (double)tanh(val);

}

double derive_sigmoid_func(double val)
{
    return (double) sigmoid_func(val) * (1-sigmoid_func(val));
}
double derive_tanh_func(double val)
{
 return (double) 1 - (tanh_func(val) * tanh_func(val));
}

void softmax_func(Matrix *m)
{
    for (size_t row=0; row< m->row_size; row++)
    {
        double sum_of_row = 0.0;
        double max_row = get_element(m,row,0);
        for (size_t col=0; col< m->col_size; col++)
        {
            double element = get_element(m, row, col);
            if (element > max_row) max_row = element;
        }   

        for (size_t col=0; col< m->col_size; col++)
        {
            double element = get_element(m, row, col);
            set_element(exp(element - max_row), m, row, col);
            sum_of_row += get_element(m, row, col);
        }  
        
        for (size_t col=0; col< m->col_size; col++)
        {
            double element = get_element(m, row, col);
            set_element(element/sum_of_row, m, row, col);
            
        } 
    }
}
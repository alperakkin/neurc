#ifndef NEURC_LOSS_H
#define NEURC_LOSS_H

#include "layer.h"

m_double mean_square_error(Matrix *output, Matrix *target);
void print_loss(m_double loss);
void gradient_loss(Matrix *output, Matrix *target, Matrix *gradient);
typedef enum {
    MSE,

} Losses;

#endif
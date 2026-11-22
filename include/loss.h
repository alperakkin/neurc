#ifndef NEURC_LOSS_H
#define NEURC_LOSS_H

#include "layer.h"

m_double mean_square_error(Matrix *output, Matrix *target);
void print_loss(m_double loss);
typedef enum {
    MSE,

} Losses;

#endif
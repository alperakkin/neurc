#ifndef NEURC_LAYER_H
#define NEURC_LAYER_H

#include <stddef.h>
#include "matrix.h"

typedef enum {
    SIGMOID,
    RELU,
    TANH,
    SOFTMAX

} Activations;


typedef struct {
    size_t input_count;
    size_t output_count;
    Matrix *weights;
    Matrix *bias;
    Matrix *stored_input;
    Matrix *pre_activation;
    Matrix *post_activation;
    Activations activation_type;
    m_double (*activation_function)(m_double);
    m_double (*derivative_function)(m_double);


} Layer;


#endif
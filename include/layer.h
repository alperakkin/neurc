#ifndef NEURC_LAYER_H
#define NEURC_LAYER_H

#define FORWARD_TRANING 0
#define FORWARD_PREDICT 1

#include "matrix.h"
#include "activation.h"
#include <stdbool.h>
#include <stdint.h>




typedef struct {
    uint32_t input_count;
    uint32_t output_count;
    uint32_t batch_size;
    Matrix *weights;
    Matrix *bias;
    Matrix *stored_input;
    Matrix *pre_activation;
    Matrix *post_activation;
    Matrix *delta;
    Matrix *dW;
    Matrix *dB;
    Matrix *dX;
    Activations activation_type;
    m_double (*activation_function)(m_double);
    m_double (*derivative_function)(m_double);
} Layer;

Layer *create_layer(uint32_t input_count, uint32_t output_count, Activations activation_func);
void layer_resize_batch(Layer *layer, uint32_t new_batch_size);
void assign_activation(Layer *layer);
void assert_created_layer(Layer *layer);
void destroy_layer(Layer *layer);

void forward(Layer *layer, const Matrix *input, bool prediction);
void backward(Layer *layer, Matrix *gradient);
void update(const Layer *layer, m_double learning_rate);


#endif
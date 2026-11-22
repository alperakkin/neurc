#ifndef NEURC_LAYER_H
#define NEURC_LAYER_H

#include <stddef.h>
#include "matrix.h"
#include "errors.h"
#include "activation.h"




typedef struct {
    size_t input_count;
    size_t output_count;
    size_t batch_size;
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

Layer *create_layer(size_t input_count, size_t output_count, size_t batch_size, Activations activation_func);
void assign_activation(Layer *layer);
void assert_created_layer(Layer *layer);
void destroy_layer(Layer *layer);
void print_activation_type(Activations type);
void print_layer(const Layer *layer, int show_matrices);

void forward(Layer *layer, const Matrix *input);
void backward(Layer *layer, Matrix *gradient);
void update(const Layer *layer, m_double learning_rate);


#endif
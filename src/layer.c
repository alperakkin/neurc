#include "layer.h"
#include "errors.h"
#include "matrix.h"
#include "activation.h"
#include "loss.h"
#include <stdlib.h>
#include <stdbool.h>



Layer *create_layer(uint32_t input_count, uint32_t output_count, Activations activation_type)
{
    if (input_count == 0 || output_count == 0) panic("Create Layer", NEURC_INCORRECT_ARGS);

    Layer *layer = calloc(1, sizeof(Layer));
    if(layer == NULL) panic("Create Layer", NEURC_ALLOC_ERROR);

    layer->input_count = input_count;
    layer->output_count = output_count;
    layer->batch_size = 0;
    layer->weights = mat_create(input_count, output_count, NULL);
    layer->bias = mat_create(1, output_count, NULL);

    layer->dW = mat_create(input_count, output_count, NULL);
    layer->dB = mat_create(1, output_count, NULL);
    layer->activation_type = activation_type;
    assert_created_layer(layer);
    random_fill(layer->weights, -1, 1);
    assign_activation(layer);


    return layer;

}

void layer_resize_batch(Layer *layer, uint32_t new_batch_size)
{
    if (layer == NULL || new_batch_size == 0) panic("Resize Batch", NEURC_INCORRECT_ARGS);
    if (layer->batch_size == new_batch_size) return;

    
    mat_destroy(layer->stored_input);
    mat_destroy(layer->pre_activation);
    mat_destroy(layer->post_activation);
    mat_destroy(layer->delta);
    mat_destroy(layer->dX);

    
    layer->batch_size = new_batch_size;
    layer->stored_input = mat_create(new_batch_size, layer->input_count, NULL);
    layer->pre_activation = mat_create(new_batch_size, layer->output_count, NULL);
    layer->post_activation = mat_create(new_batch_size, layer->output_count, NULL);
    layer->delta = mat_create(new_batch_size, layer->output_count, NULL);
    layer->dX = mat_create(new_batch_size, layer->input_count, NULL);
}

void assign_activation(Layer *layer)
{
    switch (layer->activation_type)
    {

        case RELU:
            layer->activation_function = relu_func;
            layer->derivative_function = derive_relu_func;
            break;
        case TANH:
            layer->activation_function = tanh_func;
            layer->derivative_function = derive_tanh_func;
            break;
        case SIGMOID:
            layer->activation_function = sigmoid_func;
            layer->derivative_function = derive_sigmoid_func;
            break;
    
        default:
            break;
    }

}

void assert_created_layer(Layer *layer)
{
    if(layer->weights          == NULL ||
       layer->bias             == NULL ||
       layer->dW            == NULL ||
       layer->dB            == NULL 

    ) {
        destroy_layer(layer);
        panic("Invalid Layer", NEURC_ALLOC_ERROR);
    }
}

void destroy_layer(Layer *layer)
{
    if(layer == NULL) return;
    if(layer->weights) mat_destroy(layer->weights);
    if(layer->bias) mat_destroy(layer->bias);
    if(layer->stored_input) mat_destroy(layer->stored_input);
    if(layer->pre_activation) mat_destroy(layer->pre_activation);
    if(layer->post_activation) mat_destroy(layer->post_activation);
    if(layer->delta) mat_destroy(layer->delta);
    if(layer->dW) mat_destroy(layer->dW);
    if(layer->dB) mat_destroy(layer->dB);
    if(layer->dX) mat_destroy(layer->dX);

    free(layer);
}



void compute_weights_gradient(const Layer *layer)
{
    assert_matrix_is_allocated(layer->stored_input);
    assert_matrix_is_allocated(layer->delta);
    assert_matrix_is_allocated(layer->dW);

    for (uint32_t in = 0; in < layer->input_count; in++) {
        for (uint32_t out = 0; out < layer->output_count; out++) {
            m_double sum = 0.0;

            for (uint32_t b = 0; b < layer->batch_size; b++) {
                // X^T[in, b] * delta[b, out]  -->  X[b, in] * delta[b, out]
                m_double x_val = get_element(layer->stored_input, b, in);
                m_double d_val = get_element(layer->delta, b, out);
                sum += x_val * d_val;
            }

            set_element(sum, layer->dW, in, out);
        }
    }
}

void compute_input_gradient(const Layer *layer)
{
    assert_matrix_is_allocated(layer->delta);
    assert_matrix_is_allocated(layer->weights);
    assert_matrix_is_allocated(layer->dX);


    assert_matrix_size(layer->weights, layer->input_count, layer->output_count);
    assert_matrix_size(layer->dX, layer->batch_size, layer->input_count);

    for (uint32_t b = 0; b < layer->batch_size; b++) {
        for (uint32_t in = 0; in < layer->input_count; in++) {
            m_double sum = 0.0;

            for (uint32_t out = 0; out < layer->output_count; out++) {
                // delta[b, out] * W^T[out, in]  -->  W[in, out]
                m_double d_val = get_element(layer->delta,b,out);
                m_double w_val = get_element(layer->weights, in, out);
                sum += d_val * w_val;
            }

            set_element(sum, layer->dX, b, in);
            
        }
    }
}

void compute_bias_gradient(const Layer *layer)
{
    assert_matrix_is_allocated(layer->delta);
    assert_matrix_is_allocated(layer->dB);
    assert_matrix_size(layer->dB, 1, layer->delta->col_size);

    for (uint32_t col = 0; col < layer->delta->col_size; col++) {
        m_double sum = 0.0;
        
        for (uint32_t row = 0; row < layer->delta->row_size; row++) {
            sum += get_element(layer->delta,row, col);
        }
        
        set_element(sum, layer->dB, 0, col);
    }
}

void forward(Layer *layer, const Matrix *input, bool prediction)
{
    if (layer == NULL || input == NULL) panic("Forward Pass", NEURC_ALLOC_ERROR);
    assert_matrix_size(input, input->row_size, layer->input_count);

    
    if (prediction && input->row_size != layer->batch_size) {
        layer_resize_batch(layer, input->row_size);
    }

    if (!prediction && input->row_size != layer->batch_size) {
        panic("Input row size not matched with batch size", NEURC_INCORRECT_SHAPE);
    }

    mat_copy(input, layer->stored_input);
    matmul(layer->stored_input, layer->weights, layer->pre_activation);
    add_bias(layer->pre_activation, layer->bias);
    mat_copy(layer->pre_activation, layer->post_activation);

    if (layer->activation_type == SOFTMAX) softmax_func(layer->post_activation);
    else map(layer->post_activation, layer->activation_function);
}

void update(const Layer *layer, m_double learning_rate)
{
    
    m_double scaled_lr = learning_rate / (m_double)layer->batch_size;

    mat_multiply_scalar(layer->dW, scaled_lr);
    mat_sub(layer->weights, layer->dW);

    mat_multiply_scalar(layer->dB, scaled_lr);
    mat_sub(layer->bias, layer->dB);
}



void backward(Layer *layer, Matrix *gradient)
{   
    assert_created_layer(layer);
    assert_matrix_is_allocated(gradient);
    assert_matrix_size(gradient, layer->batch_size, layer->output_count);
   
    mat_copy(layer->pre_activation, layer->delta);
    map(layer->delta, layer->derivative_function);
    hadamard(layer->delta, gradient);

    compute_weights_gradient(layer);
    compute_bias_gradient(layer);
    compute_input_gradient(layer);
 
}


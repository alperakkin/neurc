#include "layer.h"
#include "errors.h"
#include "matrix.h"
#include "activation.h"
#include "loss.h"
#include <stdlib.h>



Layer *create_layer(size_t input_count, size_t output_count, size_t batch_size, Activations activation_type)
{
    if (input_count == 0 || output_count == 0 || batch_size == 0) panic("Create Layer", NEURC_INCORRECT_ARGS);

    Layer *layer = calloc(1, sizeof(Layer));
    if(layer == NULL) panic("Create Layer", NEURC_ALLOC_ERROR);

    layer->input_count = input_count;
    layer->output_count = output_count;
    layer->batch_size = batch_size;
    layer->weights = mat_create(input_count, output_count, NULL);
    layer->bias = mat_create(1, output_count, NULL);
    layer->stored_input = mat_create(batch_size, input_count, NULL);
    layer->pre_activation = mat_create(batch_size, output_count, NULL);
    layer->post_activation = mat_create(batch_size, output_count, NULL);
    layer->delta = mat_create(batch_size, output_count, NULL);
    layer->dW = mat_create(input_count, output_count, NULL);
    layer->dB = mat_create(1, output_count, NULL);
    layer->dX = mat_create(batch_size, input_count, NULL);
    layer->activation_type = activation_type;
    assert_created_layer(layer);
    random_fill(layer->weights, -1, 1);
    assign_activation(layer);


    return layer;

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
       layer->stored_input     == NULL ||
       layer->pre_activation   == NULL ||
       layer->post_activation  == NULL ||
       layer->delta            == NULL ||
       layer->dW            == NULL ||
       layer->dB            == NULL ||
       layer->dX            == NULL 
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


void forward(Layer *layer, const Matrix *input)
{
    if(layer == NULL || input == NULL) panic("Forward Pass", NEURC_ALLOC_ERROR);
    assert_matrix_size(input, input->row_size, layer->input_count);
    if(input->row_size != layer->batch_size) panic("Input row size not matched with batch size", NEURC_INCORRECT_SHAPE);
    mat_copy(input, layer->stored_input);
    matmul(layer->stored_input, layer->weights, layer->pre_activation);
    add_bias(layer->pre_activation, layer->bias);
    mat_copy(layer->pre_activation,layer->post_activation);

    if (layer->activation_type == SOFTMAX) softmax_func(layer->post_activation);
    else map(layer->post_activation, layer->activation_function);

}

void compute_weights_gradient(const Layer *layer)
{
    assert_matrix_is_allocated(layer->stored_input);
    assert_matrix_is_allocated(layer->delta);
    assert_matrix_is_allocated(layer->dW);

    for (size_t in = 0; in < layer->input_count; in++) {
        for (size_t out = 0; out < layer->output_count; out++) {
            m_double sum = 0.0;

            for (size_t b = 0; b < layer->batch_size; b++) {
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

    for (size_t b = 0; b < layer->batch_size; b++) {
        for (size_t in = 0; in < layer->input_count; in++) {
            m_double sum = 0.0;

            for (size_t out = 0; out < layer->output_count; out++) {
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

    for (size_t col = 0; col < layer->delta->col_size; col++) {
        m_double sum = 0.0;
        
        for (size_t row = 0; row < layer->delta->row_size; row++) {
            sum += get_element(layer->delta,row, col);
        }
        
        set_element(sum, layer->dB, 0, col);
    }
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

void update(const Layer *layer, m_double learning_rate)
{
    mat_multiply_scalar(layer->dW, learning_rate);
    mat_sub(layer->weights, layer->dW);

    mat_multiply_scalar(layer->dB, learning_rate);
    mat_sub(layer->bias, layer->dB);
}
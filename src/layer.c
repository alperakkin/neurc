#include "layer.h"
#include "errors.h"
#include "matrix.h"
#include "activation.h"
#include <stdlib.h>
#include <stdio.h>

Layer *create_layer(size_t input_count, size_t output_count, size_t batch_size, Activations activation_func)
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
    layer->pre_activation = mat_create(batch_size,output_count, NULL);
    layer->post_activation = mat_create(batch_size,output_count, NULL);
    assert_created_layer(layer);
    random_fill(layer->weights, -1, 1);
    assign_activation(layer, activation_func);


    return layer;

}

void assign_activation(Layer *layer, Activations activation)
{
    switch (activation)
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
       layer->post_activation  == NULL 
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
    free(layer);
}

void print_layer(Layer *layer)
{
    printf("Layer: Shape(%zu,%zu) - Batch size: (%zu)\n", layer->input_count, layer->output_count, layer->batch_size);
    printf("--------------------------------------------------------------------\n");
    printf("--------------------------------------------------------------------\n");
    printf("Weights:\n\t");
    mat_print(layer->weights);
    printf("--------------------------------------------------------------------\n");
    printf("Bias:\n\t");
    mat_print(layer->bias);
    printf("--------------------------------------------------------------------\n");
}
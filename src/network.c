#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "network.h"
#include "layer.h"
#include "loss.h"
#include "dashboard.h"
#include "errors.h"

Network *create_network()
{
    Network *network = calloc(1, sizeof(Network));
    if (!network) return NULL;

    network->batch_size = 0;
    network->length = 0;
    network->epoch_size = 0;
    network->layers = NULL; 

    return network;
}

void set_batch_size(Network *network, uint32_t batch_size)
{   
    if (network->length == 0) panic("Add layers before setting batch size", NEURC_ALLOC_ERROR);
    
    network->batch_size = batch_size; 
    for (uint32_t index = 0; index < network->length; index++)
    {
        layer_resize_batch(network->layers[index], batch_size);
    }
}

void add_layer(Network *network, Shape shape, Activations activation)
{
    if (!network) return;

    if (network->length > 0 && network->layers[network->length - 1]->output_count != shape.input)
        panic("Current Layer Input does not match with Previous Layer Output", NEURC_INCORRECT_SHAPE);
  
    Layer **tmp = realloc(network->layers, (network->length + 1) * sizeof(Layer *));
    if (!tmp) return; 

    network->layers = tmp;

    network->layers[network->length] = create_layer(
        shape.input,
        shape.output,
        activation
    );

    assert_created_layer(network->layers[network->length]);
    network->length++;
}

void destroy_network(Network *network)
{
    if (!network) return;

    for (uint32_t i = 0; i < network->length; i++) {
        destroy_layer(network->layers[i]);
    }

    free(network->layers);
    free(network);
}

void train(Network *network, Matrix *input, Matrix *target, uint32_t epoch_size, m_double learning_rate)
{
    if (!network || !input || !target) panic("Train", NEURC_ALLOC_ERROR);
    if (input->row_size != target->row_size) panic("Train Input/Target Row Mismatch", NEURC_INCORRECT_SHAPE);

    uint32_t total_samples = input->row_size;
    
    uint32_t configured_batch = network->batch_size;
    if (configured_batch == 0 || configured_batch > total_samples) {
        configured_batch = total_samples;
        set_batch_size(network, configured_batch);
    }

    network->epoch_size = epoch_size;

    for (uint32_t epoch = 0; epoch < epoch_size; epoch++)
    {
        m_double epoch_loss = 0.0;
        uint32_t batch_count = 0;

        
        for (uint32_t start = 0; start < total_samples; start += configured_batch)
        {
            uint32_t current_batch = (start + configured_batch <= total_samples)
                                     ? configured_batch
                                     : (total_samples - start);

            
            if (network->layers[0]->batch_size != current_batch) {
                for (uint32_t index = 0; index < network->length; index++) {
                    layer_resize_batch(network->layers[index], current_batch);
                }
            }

            Matrix *batch_x = mat_create(current_batch, input->col_size, NULL);
            Matrix *batch_y = mat_create(current_batch, target->col_size, NULL);
            Matrix *gradient = mat_create(current_batch, target->col_size, NULL);

            mat_slice_rows(input, batch_x, start, current_batch);
            mat_slice_rows(target, batch_y, start, current_batch);

            
            for (uint32_t index = 0; index < network->length; index++)
            {
                if (index == 0) 
                    forward(network->layers[index], batch_x, FORWARD_TRANING);
                else
                    forward(network->layers[index], network->layers[index - 1]->post_activation, FORWARD_TRANING);
            }

            gradient_loss(network->layers[network->length - 1]->post_activation, batch_y, gradient);
            m_double batch_loss = mean_square_error(network->layers[network->length - 1]->post_activation, batch_y);
            epoch_loss += batch_loss;
            batch_count++;

            
            for (uint32_t index = network->length; index-- > 0; )
            {
                if (index == network->length - 1) 
                    backward(network->layers[index], gradient);
                else
                    backward(network->layers[index], network->layers[index + 1]->dX);

                update(network->layers[index], learning_rate);
            }

            mat_destroy(gradient);
            mat_destroy(batch_x);
            mat_destroy(batch_y);
        }

        
        network->batch_size = configured_batch;
        network->loss = epoch_loss / (m_double)batch_count;

        if (epoch % 100 == 0)
            print_layer(network->layers[network->length - 1], epoch, epoch_size, network->loss, false);
    }

    
    mat_destroy(input);
    mat_destroy(target);
}

void predict(Network *network, Matrix *input)
{
    
    for (uint32_t index = 0; index < network->length; index++)
    {
        if (index == 0) 
            forward(network->layers[index], input, FORWARD_PREDICT);
        else
            forward(network->layers[index], network->layers[index - 1]->post_activation, FORWARD_PREDICT);
    }
  
    printf("Prediction:\n");
    mat_print_limit(network->layers[network->length - 1]->post_activation, input->row_size);
}
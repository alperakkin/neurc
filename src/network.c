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
    network->epoch_size=0;
    network->layers = NULL; 

    return network;
}

void set_batch_size(Network *network, uint32_t batch_size)
{   if(network->length==0) panic("Add layers before setting batch size", NEURC_ALLOC_ERROR);
    
    for(uint32_t index=0; index<network->length; index++)
    {
        layer_resize_batch(network->layers[index], batch_size);
    }
}

void add_layer(Network *network, Shape shape, Activations activation)
{
    if (!network) return;

    if(network->length>0 && network->layers[network->length-1]->output_count!=shape.input)
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


    Matrix * gradient = mat_create(target->row_size, target->col_size, NULL);
    network->epoch_size = epoch_size;

    for(uint32_t epoch=0; epoch<epoch_size; epoch++)
    {
        
        // Forward Pass
        for(uint32_t index=0; index<network->length; index++)
        {
            if(index==0) // input_layer
            forward(network->layers[index], input, FORWARD_TRANING);
            else{
                forward(network->layers[index], network->layers[index-1]->post_activation, FORWARD_TRANING);
                
            }
        }
        gradient_loss(network->layers[network->length-1]->post_activation, target, gradient);
        m_double loss = mean_square_error(network->layers[network->length-1]->post_activation, target);
        network->loss = loss;
        // Back Propagation
        for(uint32_t index=network->length; index-->0; )
        {
            if(index==network->length-1) // output layer
            backward(network->layers[index], gradient);
            else{
                backward(network->layers[index],network->layers[index+1]->dX);
            }

            update(network->layers[index],learning_rate);
           
        }

        if(epoch % 100 == 0)
         print_layer(network->layers[network->length-1], epoch, epoch_size, loss, false);
       
        }




        //cleanup
   
        mat_destroy(input);
        mat_destroy(target);
        mat_destroy(gradient);


}

void predict(Network *network, Matrix *input)
{

     // Forward Pass
        for(uint32_t index=0; index<network->length; index++)
        {
            if(index==0) // input_layer
            forward(network->layers[index], input, FORWARD_PREDICT);
            else{
                forward(network->layers[index], network->layers[index-1]->post_activation, FORWARD_PREDICT);
                
            }
        }
      
        printf("Prediction:\n");
        mat_print_limit(network->layers[network->length-1]->post_activation, input->row_size);

}
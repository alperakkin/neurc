
#include "errors.h"
#include "layer.h"
#include "loss.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>

#define EPOCHS  15000
#define LR 0.3
#define HIDDEN_LAYERS 10

int main()
{

    seed(0);
    m_double inp_arr[8] = {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0
    };
    m_double target_arr[4] = {
        0.0,
        1.0,
        1.0,
        0.0

    };

    Matrix *input = mat_create(4, 2, inp_arr);
    Matrix *target = mat_create(4, 1, target_arr);
    Matrix * gradient = mat_create(target->row_size, target->col_size, NULL);
    Layer *input_layer = create_layer(2, HIDDEN_LAYERS, 4, SIGMOID);
    Layer *output_layer = create_layer(HIDDEN_LAYERS, 1, 4, SIGMOID);
    

    for(size_t epoch=0; epoch<EPOCHS; epoch++)
    {
        
    forward(input_layer, input, FORWARD_TRANING);
    forward(output_layer, input_layer->post_activation, FORWARD_TRANING);
    m_double loss = mean_square_error(output_layer->post_activation, target);
    gradient_loss(output_layer->post_activation, target, gradient);
    backward(output_layer, gradient);
    backward(input_layer,output_layer->dX);
    update(input_layer, LR);
    update(output_layer,LR);
 
    print_layer(output_layer,epoch,loss, false);
    }


    printf("Prediction:\n");
    mat_print_limit(output_layer->post_activation, input->row_size);

    //cleanup
    destroy_layer(input_layer);
    destroy_layer(output_layer);
    mat_destroy(input);
    mat_destroy(target);
    mat_destroy(gradient);

    return NEURC_OK;
}

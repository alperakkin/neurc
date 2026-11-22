
#include "errors.h"
#include "layer.h"
#include "loss.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>

int main()
{

    seed(42);
    m_double inp_arr[2] = {
        0.9, 0.2
    };
    m_double target_arr[1] = {
        1
    };

    Matrix *input = mat_create(1, 2, inp_arr);
    Matrix *target = mat_create(1, 1, target_arr);
    Matrix * gradient = mat_create(target->row_size, target->col_size, NULL);
    Layer *layer = create_layer(2, 1, 1, SIGMOID);
    

    forward(layer, input);
    
    
    m_double loss = mean_square_error(layer->post_activation, target);
    print_loss(loss); 
    gradient_loss(layer->post_activation, target, gradient);
    backward(layer, gradient);
    update(layer, 0.51);
 
    print_layer(layer, true);



    //cleanup
    destroy_layer(layer);
    mat_destroy(input);
    mat_destroy(target);
    mat_destroy(gradient);

    return NEURC_OK;
}


#include <string.h>
#include "errors.h"
#include "utils.h"
#include "network.h"

#define EPOCHS  250000
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


    Network *network = create_network(4);
    add_layer(network,(Shape){.input=2,  .output=10}, SIGMOID);
    add_layer(network,(Shape){.input=10, .output=1}, SIGMOID);

    train(network, input, target, EPOCHS, LR);

    m_double q_arr[4] = {
        0.5, 0.5,
        1.0, 0.9

    
    };
    Matrix *q = mat_create(2, 2, q_arr);
    predict(network, q);
    destroy_network(network);
   
    


 
    return NEURC_OK;
}

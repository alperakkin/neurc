
#include <string.h>
#include "errors.h"
#include "utils.h"
#include "network.h"
#include "filesystem.h"


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


    // Network *network = create_network();
    // add_layer(network,(Shape){.input=2,  .output=10}, SIGMOID);
    // add_layer(network,(Shape){.input=10, .output=1}, SIGMOID);
    // set_batch_size(network, 4);


    // train(network, input, target, EPOCHS, LR);

    m_double q_arr[4] = {
        0.5, 0.5,
        1.0, 0.9

    
    };
    Matrix *q = mat_create(2, 2, q_arr);
    // printf("Trained\n");
    // predict(network, q);
    // save_model("data/xor", network);
    // destroy_network(network);
    
    
    
    Network *loaded_net = load_model("data/xor");
    set_batch_size(loaded_net, 4);
    
    printf("PreTrained\n");
    predict(loaded_net, q);
    destroy_network(loaded_net);
   
    


 
    return NEURC_OK;
}

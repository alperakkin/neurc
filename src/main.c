
#include "errors.h"
#include "layer.h"
#include "utils.h"

int main()
{

    seed(0);
    m_double XOR[8] = {
        0, 0,
        0, 1,
        1, 0,
        1, 1
    };

    Matrix *input = mat_create(4, 2, XOR);

    Layer *layer = create_layer(2, 1, 4, SIGMOID);
    fill(layer->bias, 0.25f);
    forward(layer, input);

    print_layer(layer);
    destroy_layer(layer);
    mat_destroy(input);

    return NEURC_OK;
}



#include "errors.h"
#include "layer.h"
#include "utils.h"

int main()
{
    seed(45);
    Layer *input = create_layer(10, 5, 10, SIGMOID);
    print_layer(input);
    destroy_layer(input);

    return NEURC_OK;
}


#ifndef NEURC_NETWORK_H
#define NEURC_NETWORK_H

#include <stddef.h>
#include "layer.h"

typedef struct {
    uint32_t input;
    uint32_t output;
} Shape;

typedef struct {
    uint32_t length;
    uint32_t batch_size;
    uint32_t epoch_size;
    m_double loss;
    Layer **layers;
} Network;

Network *create_network();
void destroy_network(Network *network);
void set_batch_size(Network *network, uint32_t batch_size);
void add_layer(Network *network, Shape shape, Activations activation);
void train(Network *network, Matrix *input, Matrix *target, uint32_t epoch_size, m_double learning_rate);
void predict(Network *network, Matrix *input);
#endif
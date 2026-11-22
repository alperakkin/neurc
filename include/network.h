#ifndef NEURC_NETWORK_H
#define NEURC_NETWORK_H

#include <stddef.h>
#include "layer.h"

typedef struct {
    size_t input;
    size_t output;
} Shape;

typedef struct {
    size_t length;
    size_t batch_size;
    m_double loss;
    Layer **layers;
} Network;

Network *create_network(size_t batch_size);
void destroy_network(Network *network);
void add_layer(Network *network, Shape shape, Activations activation);
void train(Network *network, Matrix *input, Matrix *target, size_t epoch_size, m_double learning_rate);
void predict(Network *network, Matrix *input);
#endif
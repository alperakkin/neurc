#ifndef NEURC_ACTIVATION_H
#define NEURC_ACTIVATION_H
#include "matrix.h"

double sigmoid_func(double val);
double relu_func(double val);
double tanh_func(double val);
void softmax_func(Matrix *m);
double derive_sigmoid_func(double val);
double derive_tanh_func(double val);
double derive_relu_func(double val);

#endif
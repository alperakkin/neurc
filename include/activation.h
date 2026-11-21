#ifndef NEURC_ACTIVATION_H
#include "matrix.h"
#define NEURC_ACTIVATION_H

double sigmoid_func(double val);
double relu_func(double val);
double tanh_func(double val);
void softmax_func(Matrix *m);
#endif
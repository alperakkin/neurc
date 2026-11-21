#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "activation.h"
#include "layer.h"

#define ROW 4
#define COL 3


m_double square(m_double val)
{
    return val*val;
}



int main()
{
    srand((unsigned int)time(NULL)); 
  

    Matrix *m1 = mat_create(ROW, COL, NULL);
    random_fill(m1);  
    
    double bias_arr[3] = {
        10, 20 , 30
    };

    Matrix *bias = mat_create(1, 3, bias_arr);
    
    //map(m1, tanh_func);
    //softmax_func(m1);
    mat_print(m1);
    mat_print(bias);
    add_bias(m1, bias);
    mat_print(m1);
    


    mat_destroy(m1);
    mat_destroy(bias);
  
   

    return NEURC_OK;
}


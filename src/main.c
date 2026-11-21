#include "matrix.h"
#include "activation.h"

#define ROW 4
#define COL 3


m_double square(m_double val)
{
    return val*val;
}



int main()
{

    m_double arr1[ROW*COL] = {
        0, 0.1, 0.2,
        0.3, 0.4, 0.5,
        -0.6, 0.7, 0.8,
        -500,2000,100
    };



    Matrix *m1 = mat_create(ROW, COL, arr1);
   

    
    //map(m1, tanh_func);
    softmax_func(m1);
    mat_print(m1);
    


    mat_destroy(m1);
  
   

    return NEURC_OK;
}


#include "matrix.h"


m_double square(m_double val)
{
    return val*val;
}

int main()
{

    m_double arr1[6] = {
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
    };



    Matrix *m1 = mat_create(3, 2, arr1);
   

    
    map(m1, square);
    mat_print(m1);
    


    mat_destroy(m1);
  
   

    return NEURC_OK;
}


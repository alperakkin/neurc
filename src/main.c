#include "matrix.h"


int main()
{

    m_double arr1[6] = {
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
    };



    Matrix *m1 = mat_create(3, 2, arr1);
    Matrix *res  = mat_create(2, 3, NULL);

    
    transpose(m1, res);
    mat_print(m1);
    mat_print(res);


    mat_destroy(m1);
    mat_destroy(res);
   

    return NEURC_OK;
}


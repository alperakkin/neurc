#include "matrix.h"


int main()
{

    m_double arr1[6] = {
        1, 1, 1,
        2, 2, 2,
    };

    m_double arr2[8] = {
        3, 3,
        3, 3,
        3, 3,
        3, 3,
    };

    Matrix *m1 = mat_create(3, 2, &arr1);
    Matrix *m2 = mat_create(2, 4, &arr2);
    Matrix *res  = mat_create(3, 4, NULL);

    
    matmul(m1, m2, res);
    mat_print(m1);
    mat_print(m2);
    mat_print(res);


    mat_destroy(m1);
    mat_destroy(m2);
    mat_destroy(res);
   

    return NEURC_OK;
}


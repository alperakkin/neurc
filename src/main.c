#include <stdio.h>
#include "matrix.h"

int main()
{

    Matrix *m = mat_create(2,3);
    Matrix *m2 = mat_create(2,3);
  
    for (int col=0; col<3 ; col++)
    {

        set_element((m_double)(col+1), m, 1, col);
        set_element((m_double)(col+2), m2, 1, col);
    }
  
    
    mat_add(m, m2);
    
    mat_print(m);




    mat_destroy(m);
   

    return NEURC_OK;
}
#include <stdio.h>
#include "matrix.h"

int main()
{

    Matrix *m = mat_create(2,3);
  

    set_element(1.0f, m, 1, 1);
    set_element(2.0f, m, 1, 2);
    mat_print(m);
    mat_destroy(m);
   

    return NEURC_OK;
}
#include <stdio.h>
#include "matrix.h"

int main()
{

    Matrix *m = mat_create(2,3);
    if (m == NULL)
    {
        print_error(NEURC_ALLOC_ERROR);
        return NEURC_ALLOC_ERROR;
    }
    ErrorTypes err = mat_print(m);
    if (err) 
    {
        print_error(err);
        return err;
    }

    err = mat_destroy(m);
    if(err)
    {
        print_error(err);
        return err;
    }
    return 0;
}
#include <stdio.h>
#include "matrix.h"

int main()
{

    Matrix *m = create_matrix(2,3);
    print_matrix(m);
    destroy_matrix(m);
    return 0;
}
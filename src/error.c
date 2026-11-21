#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

void panic(const char* function_name, ErrorTypes err)
{
    fprintf(stderr, "\033[31m\n");
    fprintf(stderr, "Error in %s: ", function_name); 
    
    switch (err)
    {
    case NEURC_INCORRECT_ARGS:
        fprintf(stderr, "Incorrect Matrix Arguments");
        break;
    case NEURC_INCORRECT_SHAPE:
        fprintf(stderr, "Incorrect Matrix Shape");
        break;
    case NEURC_INCORRECT_SIZE:
        fprintf(stderr, "Incorrect Matrix Size");
        break;
    case NEURC_MATMUL_NOT_MATCHED:
        fprintf(stderr, "Matrix columns not matched for MATMUL operations");
        break;   
    default:
        fprintf(stderr, "Undefined Error");
        break;
    }
    
    fprintf(stderr, "\033[0m\n");
    exit(err);
}
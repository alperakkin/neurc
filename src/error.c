#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

void panic(const char* function_name, ErrorTypes err)
{
    fprintf(stderr, "\033[31m\n");
    fprintf(stderr, "Error ["); 

    switch (err)
    {
    case NEURC_INCORRECT_ARGS:
        fprintf(stderr, "Incorrect Matrix Arguments");
        break;
    case NEURC_ALLOC_ERROR:
        fprintf(stderr, "Matrix is not allocated");
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
    case NEURC_FILE_ERROR:
        fprintf(stderr, "File operation not allowed");
        break;
    default:
        fprintf(stderr, "Undefined Error");
        break;
    }
    fprintf(stderr, "]: %s", function_name); 
    fprintf(stderr, "\033[0m\n");
    exit(err);
}
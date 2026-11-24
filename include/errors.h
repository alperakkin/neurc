#ifndef NEURC_ERRORS_H
#define NEURC_ERRORS_H


typedef enum {
    NEURC_OK,
    NEURC_ALLOC_ERROR,
    NEURC_INCORRECT_SHAPE,
    NEURC_MATMUL_NOT_MATCHED,
    NEURC_INCORRECT_SIZE,
    NEURC_INCORRECT_ARGS,
    NEURC_TYPE_ERROR,
    NEURC_FILE_ERROR
    
} ErrorTypes;

void panic(const char* function_name, ErrorTypes err);

#endif
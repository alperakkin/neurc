#ifndef NEURC_FILESYSTEM_H
#define NEURC_FILESYSTEM_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "network.h"

#define FILE_SIGNATURE "NEUR"
#define FILE_FOOTER    "RUEN"
#define SIG_LEN        4
#define VERSION 1
#define BYTE_ORDER_MARK 0x01020304


typedef struct {
    char signature[4];
    uint32_t version;
    uint32_t byte_order_mark;
    uint32_t m_double_size;
    uint32_t layer_length;
    uint32_t total_epochs;
    m_double loss; //8 bytes

} ModelHeader;

void save_model(char* model_name, Network *Network);
Network *load_model(char* model_name);





#endif
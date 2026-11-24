#include "errors.h"
#include "filesystem.h"

#include <stdio.h>


void get_path(char *model_path, size_t path_size, const char *model_name)
{
    if (model_path == NULL || model_name == NULL || path_size == 0) {
        panic("Invalid path arguments", NEURC_INCORRECT_ARGS);
    }

    const char *suffix = ".neurc";
    int written = snprintf(model_path, path_size, "%s%s", model_name, suffix);

    if (written < 0 || (size_t)written >= path_size) {
        panic("Given model name is too big (max 256)", NEURC_FILE_ERROR);
    }
}

void save_model(char* model_name, Network *network)
{
    char model_path[256];
    get_path(model_path, sizeof(model_path), model_name);
    ModelHeader header = {
        .byte_order_mark = BYTE_ORDER_MARK,
        .m_double_size = sizeof(m_double),
        .layer_length = network->length,
        .total_epochs = network->epoch_size,
    };
    
    memcpy(header.signature, FILE_SIGNATURE, SIG_LEN);
    header.version = VERSION;


    FILE *fp = fopen(model_path, "wb");
    if (fp == NULL) panic("Model file could not be created", NEURC_ALLOC_ERROR);

    fwrite(&header.signature, sizeof(header.signature), 1, fp);
    fwrite(&header.version, sizeof(header.version), 1, fp);
    fwrite(&header.byte_order_mark, sizeof(header.byte_order_mark), 1, fp);
    fwrite(&header.m_double_size, sizeof(header.m_double_size), 1, fp);
    if(sizeof(network->length) > sizeof(uint32_t)) panic("Network has to many layers.", NEURC_TYPE_ERROR);
    fwrite(&header.layer_length, sizeof(header.layer_length), 1, fp);
    fwrite(&header.total_epochs, sizeof(header.total_epochs), 1, fp);
    //start layers
    for(uint32_t index=0; index<network->length; index++)
    {
        //input_count
        fwrite(&network->layers[index]->input_count, sizeof(uint32_t), 1, fp);
        //output_count
        fwrite(&network->layers[index]->output_count, sizeof(uint32_t), 1, fp);
        //activation_type
        fwrite(&network->layers[index]->activation_type, sizeof(uint32_t), 1, fp);
        // weight_row_size
        fwrite(&network->layers[index]->weights->row_size, sizeof(uint32_t), 1, fp);
        // weight_col_size
        fwrite(&network->layers[index]->weights->col_size, sizeof(uint32_t), 1, fp);
        // weight data
        for(uint32_t w_index=0; w_index < get_size(network->layers[index]->weights); w_index++)
        {
            fwrite(&network->layers[index]->weights->data[w_index], sizeof(m_double),1, fp);
        }
        // bias_row_size
        fwrite(&network->layers[index]->bias->row_size, sizeof(uint32_t), 1, fp);
        // bias_col_size
        fwrite(&network->layers[index]->bias->col_size, sizeof(uint32_t), 1, fp);
        // bias data
        for(uint32_t w_index=0; w_index < get_size(network->layers[index]->bias); w_index++)
        {
            fwrite(&network->layers[index]->bias->data[w_index], sizeof(m_double),1, fp);
        }


    }
    char footer[4];
    memcpy(footer, FILE_FOOTER, SIG_LEN);
    fwrite(footer, sizeof(footer),1, fp);
   
    fclose(fp);

}
Network *load_model(char* model_name){
    Network *network = create_network();
    if (network == NULL) panic("Failed to allocate network", NEURC_ALLOC_ERROR);
    char model_path[256];
    get_path(model_path,sizeof(model_path), model_name);
    FILE *fp = fopen(model_path, "rb");
    if (fp == NULL) {
        panic("Model file could not be opened", NEURC_FILE_ERROR);
    }

    ModelHeader header = {0};

    if (fread(&header.signature, sizeof(header.signature), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file signature", NEURC_FILE_ERROR);
    }

    if (fread(&header.version, sizeof(header.version), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file version", NEURC_FILE_ERROR);
    }


    if (memcmp(header.signature, FILE_SIGNATURE, 4) != 0) {
        fclose(fp);
        panic("Invalid file format [signature]", NEURC_FILE_ERROR);
    }


    if (fread(&header.byte_order_mark, sizeof(header.byte_order_mark), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file byte order", NEURC_FILE_ERROR);
    }


    if (header.byte_order_mark != (uint32_t)BYTE_ORDER_MARK) {
        fclose(fp);
        panic("Invalid file format [byte order]", NEURC_FILE_ERROR);
    }
    
    if (fread(&header.m_double_size, sizeof(header.m_double_size), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
    }

    if (header.m_double_size != sizeof(m_double)) {
        fclose(fp);
        panic("Invalid file format [m_double]", NEURC_FILE_ERROR);
    }

    if (fread(&header.layer_length, sizeof(header.layer_length), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
    }


    if (fread(&header.total_epochs, sizeof(header.total_epochs), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
    }

    if (sizeof(header.total_epochs) != sizeof(uint32_t)) {
        fclose(fp);
        panic("Invalid file format [total_epochs]", NEURC_FILE_ERROR);
    }

    network->epoch_size = header.total_epochs;

    //start layers
    for(uint32_t index=0; index<header.layer_length; index++)
    {
        //input_count
        uint32_t input_count;
        if (fread(&input_count, sizeof(input_count), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
        }

        if (sizeof(input_count) != sizeof(uint32_t)) {
        fclose(fp);
        panic("Invalid file format [layer->input_count]", NEURC_FILE_ERROR);
        }



        //output_count
        uint32_t output_count;
        if (fread(&output_count, sizeof(output_count), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
        }

        if (sizeof(output_count) != sizeof(uint32_t)) {
        fclose(fp);
        panic("Invalid file format [layer->output_count]", NEURC_FILE_ERROR);
        }
        //activation_type
        uint32_t activation;
        if (fread(&activation, sizeof(activation), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
        }

        if (sizeof(activation) != sizeof(uint32_t)) {
        fclose(fp);
        panic("Invalid file format [layer->activation]", NEURC_FILE_ERROR);
        }
        // weight_row_size
        uint32_t row_size;
        if (fread(&row_size, sizeof(row_size), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
        }

        if (sizeof(row_size) != sizeof(uint32_t)) {
        fclose(fp);
        panic("Invalid file format [layer->weight->row_size]", NEURC_FILE_ERROR);
        }
        // weight_col_size
        uint32_t col_size;
        if (fread(&col_size, sizeof(col_size), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
        }

        if (sizeof(col_size) != sizeof(uint32_t)) {
        fclose(fp);
        panic("Invalid file format [layer->weight->col_size]", NEURC_FILE_ERROR);
        }
  
        
        add_layer(network,(Shape){.input=input_count,  .output=output_count}, activation);
        
        
        // weight data
        for(uint32_t w_index=0;w_index<row_size*col_size;w_index++)
        {
            m_double w;
            if (fread(&w, sizeof(m_double), 1, fp) != 1) {
            fclose(fp);
            panic("Failed to read file data type", NEURC_FILE_ERROR);
            }

            if (sizeof(w) != sizeof(m_double)) {
                fclose(fp);
                panic("Invalid file format [layer->w]", NEURC_FILE_ERROR);
            }

            network->layers[index]->weights->data[w_index] = w;
        }

        // bias_row
        if (fread(&row_size, sizeof(row_size), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
        }

        if (sizeof(row_size) != sizeof(uint32_t)) {
        fclose(fp);
        panic("Invalid file format [layer->bias->row_size]", NEURC_FILE_ERROR);
        }
        // bias_col_size
       
        if (fread(&col_size, sizeof(col_size), 1, fp) != 1) {
        fclose(fp);
        panic("Failed to read file data type", NEURC_FILE_ERROR);
        }

        if (sizeof(col_size) != sizeof(uint32_t)) {
        fclose(fp);
        panic("Invalid file format [layer->weight->bias]", NEURC_FILE_ERROR);
        }

  
        
        // bias data
        for(uint32_t b_index=0;b_index<row_size*col_size;b_index++)
        {
            m_double b;
            if (fread(&b, sizeof(m_double), 1, fp) != 1) {
            fclose(fp);
            panic("Failed to read file data type", NEURC_FILE_ERROR);
            }

            if (sizeof(b) != sizeof(m_double)) {
                fclose(fp);
                panic("Invalid file format [layer->b]", NEURC_FILE_ERROR);
            }

            network->layers[index]->bias->data[b_index] = b;
        }
        }
    
    char footer[4];

    fread(&footer, SIG_LEN, 1, fp);
    if (memcmp(footer, FILE_FOOTER, SIG_LEN) != 0) {
        panic("Invalid footer", NEURC_FILE_ERROR);

    }
   

    
    fclose(fp);

    return network;

}
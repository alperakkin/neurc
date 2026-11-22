#include <stdio.h>
#include <math.h>
#include "dashboard.h"



void training_ui_init(void) {
    printf(ANSI_CLEAR_SCREEN ANSI_CURSOR_HOME ANSI_HIDE_CURSOR);
    fflush(stdout);
}

void training_ui_finish(void) {
    printf(ANSI_SHOW_CURSOR "\n");
    fflush(stdout);
}

void print_activation_type(Activations type)
{
    printf(ANSI_BOLD "Activation : " ANSI_RESET);
    switch (type)
    {
    case SIGMOID:
        printf(ANSI_GREEN "SIGMOID" ANSI_RESET);
        break;
    case RELU:
        printf(ANSI_GREEN "RELU" ANSI_RESET);
        break;
    case TANH:
        printf(ANSI_GREEN "TANH" ANSI_RESET);
        break;
    case SOFTMAX:
        printf(ANSI_GREEN "SOFTMAX" ANSI_RESET);
        break;
    default:
        printf(ANSI_RED "Undefined" ANSI_RESET);
        break;
    }
    printf("\n");
}


static void print_matrix_summary(const char *label, const Matrix *m, const char *color)
{
    if (!m || !m->data) {
        printf("  %s%-16s" ANSI_RESET " │ " ANSI_DIM "NULL / Empty\n" ANSI_RESET, color, label);
        return;
    }

    size_t total = m->row_size * m->col_size;
    double min_v = m->data[0];
    double max_v = m->data[0];
    double sum   = 0.0;
    int has_nan  = 0;

    for (size_t i = 0; i < total; i++) {
        double val = m->data[i];
        if (isnan(val) || isinf(val)) has_nan = 1;
        if (val < min_v) min_v = val;
        if (val > max_v) max_v = val;
        sum += val;
    }

    double mean_v = (total > 0) ? (sum / total) : 0.0;

    printf("  %s%-16s" ANSI_RESET " │ " ANSI_DIM "(%4zu x %-4zu)" ANSI_RESET
           " │ min: %8.4f │ max: %8.4f │ mean: %8.4f",
           color, label, m->row_size, m->col_size, min_v, max_v, mean_v);

    
    if (has_nan) {
        printf("  " ANSI_RED ANSI_BOLD "[! NaN/Inf !]" ANSI_RESET);
    } else if (fabs(min_v) < 1e-9 && fabs(max_v) < 1e-9) {
        printf("  " ANSI_DIM "[All Zeros]" ANSI_RESET);
    }
    printf("\n");
}


static void print_progress_bar(size_t current_epoch, size_t total_epochs) {
    const int bar_width = 36;
    double progress = (total_epochs > 0) ? (double)current_epoch / (double)total_epochs : 0.0;
    if (progress > 1.0) progress = 1.0;

    int filled = (int)(progress * bar_width);

    printf("  " ANSI_BOLD "Progress   : " ANSI_RESET ANSI_CYAN "[");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            printf(ANSI_GREEN "█");
        } else {
            printf(ANSI_DIM "░");
        }
    }
    printf(ANSI_RESET ANSI_CYAN "] " ANSI_BOLD ANSI_YELLOW "%5.1f%%" ANSI_RESET " (%zu/%zu)\n",
           progress * 100.0, current_epoch, total_epochs);
}

void print_layer(const Layer *layer, size_t epoch, size_t total_epochs, m_double loss, int show_matrices)
{
    if (!layer) {
        printf(ANSI_RED "[Error: NULL Layer]\n" ANSI_RESET);
        return;
    }
    static int is_initialized = 0;
    if (!is_initialized) {
        printf(ANSI_CLEAR_SCREEN ANSI_HIDE_CURSOR);
        is_initialized = 1;
    }

   
    printf(ANSI_CURSOR_HOME);

    size_t total_params = (layer->input_count * layer->output_count) + layer->output_count;

  
    printf(ANSI_CYAN ANSI_BOLD "╭──────────────────────────────────────────────────────────────────────────────╮\n");
    printf("│ EPOCH: %-6zu │ LOSS: ", epoch);

    if (isnan(loss) || isinf(loss)) {
        printf(ANSI_RED "%-12.6f" ANSI_CYAN, (double)loss);
    } else {
        printf(ANSI_YELLOW "%-12.6f" ANSI_CYAN, (double)loss);
    }

    printf(" │ Shape: (%zu ──▶ %zu) │ Params: %-9zu │\n",
           layer->input_count, layer->output_count, total_params);
    printf("╰──────────────────────────────────────────────────────────────────────────────╯\n" ANSI_RESET);

  
    print_progress_bar(epoch, total_epochs);
    printf(ANSI_DIM "  ────────────────────────────────────────────────────────────────────────────\n" ANSI_RESET);


    printf("  " ANSI_BOLD "Activation : " ANSI_RESET);
    switch (layer->activation_type) {
        case SIGMOID: printf(ANSI_GREEN "%-10s" ANSI_RESET, "SIGMOID"); break;
        case RELU:    printf(ANSI_GREEN "%-10s" ANSI_RESET, "RELU");    break;
        case TANH:    printf(ANSI_GREEN "%-10s" ANSI_RESET, "TANH");    break;
        case SOFTMAX: printf(ANSI_GREEN "%-10s" ANSI_RESET, "SOFTMAX"); break;
        default:      printf(ANSI_RED   "%-10s" ANSI_RESET, "Undefined"); break;
    }

    printf(" │  " ANSI_BOLD "Batch Size : " ANSI_RESET ANSI_YELLOW "%zu\n" ANSI_RESET, layer->batch_size);
    printf(ANSI_DIM "  ────────────────────────────────────────────────────────────────────────────\n" ANSI_RESET);

    if (!show_matrices) {
        print_matrix_summary("Weights",         layer->weights,         ANSI_MAGENTA);
        print_matrix_summary("Bias",            layer->bias,            ANSI_MAGENTA);
        print_matrix_summary("Pre-Activation",  layer->pre_activation,  ANSI_CYAN);
        print_matrix_summary("Post-Activation", layer->post_activation, ANSI_CYAN);
        print_matrix_summary("Delta (Grad)",    layer->delta,           ANSI_YELLOW);
        
      
        print_matrix_summary("Grad dW",         layer->dW,              ANSI_DIM);
        print_matrix_summary("Grad dB",         layer->dB,              ANSI_DIM);
        print_matrix_summary("Grad dX",         layer->dX,              ANSI_DIM);
    } else {
        printf(ANSI_MAGENTA ANSI_BOLD "\n▸ Weights:\n" ANSI_RESET);
        mat_print(layer->weights);

        printf(ANSI_MAGENTA ANSI_BOLD "\n▸ Bias:\n" ANSI_RESET);
        mat_print(layer->bias);

        printf(ANSI_CYAN ANSI_BOLD "\n▸ Pre-Activation:\n" ANSI_RESET);
        mat_print(layer->pre_activation);

        printf(ANSI_CYAN ANSI_BOLD "\n▸ Post-Activation:\n" ANSI_RESET);
        mat_print(layer->post_activation);

        printf(ANSI_YELLOW ANSI_BOLD "\n▸ Delta:\n" ANSI_RESET);
        mat_print(layer->delta);

        printf(ANSI_DIM ANSI_BOLD "\n▸ dW:\n" ANSI_RESET);
        mat_print(layer->dW);

        printf(ANSI_DIM ANSI_BOLD "\n▸ dB:\n" ANSI_RESET);
        mat_print(layer->dB);

        printf(ANSI_DIM ANSI_BOLD "\n▸ dX:\n" ANSI_RESET);
        mat_print(layer->dX);
    }

    printf(ANSI_DIM "════════════════════════════════════════════════════════════════════════════════\n" ANSI_RESET);
    
   
    fflush(stdout);
}
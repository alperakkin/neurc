#ifndef NEURC_DASHBOARD_H
#define NEURC_DASHBOARD_H

#include "layer.h"

#define ANSI_CLEAR_SCREEN "\x1b[2J"
#define ANSI_CURSOR_HOME  "\x1b[H"
#define ANSI_HIDE_CURSOR  "\x1b[?25l"
#define ANSI_SHOW_CURSOR  "\x1b[?25h"

#define ANSI_RESET   "\x1b[0m"
#define ANSI_BOLD    "\x1b[1m"
#define ANSI_DIM     "\x1b[2m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_RED     "\x1b[31m"
#define MAT_PRINT_ALL 0

void training_ui_init(void);
void training_ui_finish(void);
void print_activation_type(Activations type);
void print_layer(const Layer *layer, uint32_t epoch, uint32_t total_epochs, m_double loss, int show_matrices);
void mat_print_limit(const Matrix *m, uint32_t max_rows);
#endif
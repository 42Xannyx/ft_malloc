#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>
#include <unistd.h>

#include "alloc.h"

#ifdef DEBUG
#define ANSI_COLOR_ORANGE "\x1b[38;5;208m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define DEBUG_PRINT(fmt, ...)                                                  \
  fprintf(stderr,                                                              \
          ANSI_COLOR_ORANGE "(LOG) " ANSI_COLOR_RESET "%s:%d:%s(): " fmt,      \
          __FILE__, __LINE__, __func__, __VA_ARGS__)
#define DEBUG_PRINT_SIMPLE(msg)                                                \
  fprintf(stderr,                                                              \
          ANSI_COLOR_ORANGE "(LOG) " ANSI_COLOR_RESET "%s:%d:%s(): %s\n",      \
          __FILE__, __LINE__, __func__, msg)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#define DEBUG_PRINT_SIMPLE(msg) ((void)0)
#endif

#ifdef DEBUG
void print_sizes(void);
void print_block(t_block *block);
void print_heap(t_heap *heap, bool show_blocks);
void print_block_count(t_amount block_count);
#endif // DEBUG

#endif // DEBUG_H

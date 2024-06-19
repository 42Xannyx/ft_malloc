#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>
#include <unistd.h>

#include "alloc.h"

#ifdef DEBUG
void print_sizes(void);
void print_block(t_block *block);
void print_heap(t_heap *heap);
#endif // DEBUG

#endif // DEBUG_H

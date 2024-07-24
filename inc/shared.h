#ifndef SHARED
#define SHARED

#include "alloc.h"

void check_block_integrity(t_block *block);

bool blocks_inuse(t_heap *heap);

t_amount amount_of_unused_space(t_heap *heap);

bool find_enough_unused_space(t_heap *heap, t_amount amount);

#endif // !SHARED

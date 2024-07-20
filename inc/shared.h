#ifndef SHARED
#define SHARED

#include "alloc.h"

void check_block_integrity(t_block *block);

bool blocks_inuse(t_heap *heap);

size_t amount_of_unused_blocks(t_heap *heap);

#endif // !SHARED

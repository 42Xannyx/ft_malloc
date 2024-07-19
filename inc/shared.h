#ifndef SHARED
#define SHARED

#include "alloc.h"

void check_block_integrity(t_block *block);

bool blocks_inuse(t_heap *heap);

bool any_block_not_inuse(t_heap *heap);

#endif // !SHARED

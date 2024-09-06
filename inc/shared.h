#ifndef SHARED
#define SHARED

#include "alloc.h"

void check_buffer_overflow(t_block *block);

bool blocks_inuse(t_heap *heap);

t_heap *find_heap_for_block(t_block *);

bool find_enough_unused_space(t_heap *heap, const size_t);

inline bool create_large_heap(const size_t n) {
  return n > (size_t)SMALL_HEAP_ALLOCATION_SIZE;
}

#endif // !SHARED

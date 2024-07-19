#include "alloc.h"

#include <stdlib.h>

void check_block_integrity(t_block *block) {
  size_t total_size = sizeof(t_block) + block->size;
  size_t *end_canary = (size_t *)((char *)block + total_size);
  if (*end_canary != BLOCK_MAGIC) {
    fprintf(stderr, "Buffer overflow detected in block %p\n", (void *)block);
    abort();
  }
}
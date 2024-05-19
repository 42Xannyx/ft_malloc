#ifndef ALLOC
#define ALLOC

#include <stdbool.h>
#include <stddef.h>

/**
 * @struct block
 * @brief Structure representing a block in the heap.
 *
 * The `block` structure is used to manage memory in a heap allocator. It
 * contains metadata about each block of memory, including pointers to the
 * previous and next blocks, the size of the block, and a flag indicating
 * whether the block is free.
 */
typedef struct block {
  struct block *prev; /**< Pointer to the previous block in the heap. */
  struct block *next; /**< Pointer to the next block in the heap. */
  size_t size;        /**< Size of the block of memory. */
  bool freed;         /**< Flag indicating whether the block is free. */
} t_block;

#endif // !ALLOC

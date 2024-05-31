#ifndef ALLOC
#define ALLOC

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_HEAP_ALLOCATION_SIZE (8 * getpagesize())
#define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)

typedef intptr_t word_t;

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
  size_t size;        /**< Size of the block of memory. */
  bool inuse;         /**< Flag indicating whether the block is free. */
  struct block *next; /**< Pointer to the next block in the heap. */
} t_block;

/**
 * @struct heap
 * @brief Structure representing the heap.
 *
 * The `heap` structure is used to manage the overall heap. It contains
 * metadata about the heap, including pointers to the previous and next heaps,
 * the total size of the heap, the size of the free memory in the heap, and the
 * number of blocks in the heap.
 */
typedef struct heap {
  size_t total_size;  /**< Total size of the heap. */
  size_t free_size;   /**< Size of the free memory in the heap. */
  size_t block_count; /**< Number of blocks in the heap. */
  struct heap *next;  /**< Pointer to the next heap structure. */
} t_heap;

// ****** Functions ****** //

__attribute__((warn_unused_result)) inline size_t align(size_t n) {
  return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
}

#endif // !ALLOC

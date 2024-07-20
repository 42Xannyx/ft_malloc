#ifndef ALLOC
#define ALLOC

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#ifdef __APPLE__
#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_HEAP_ALLOCATION_SIZE (8 * getpagesize())
#define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)
#endif

#ifdef linux
#define TINY_HEAP_ALLOCATION_SIZE (4 * sysconf(_SC_PAGESIZE))
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_HEAP_ALLOCATION_SIZE (8 * sysconf(_SC_PAGESIZE))
#define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)
#endif

typedef intptr_t word_t;

/* Let's say you allocate 10 bytes. I will give you 79 bytes to be used. */

/*
 * Magic number used as a canary value to detect memory corruption.
 * Properties of 0xDEADBEEF:
 * 1. Memorable: Spells "DEAD BEEF" in hexadecimal.
 * 2. Unlikely to occur accidentally: Not a common bit pattern in memory.
 * 3. Detectable: Easy to spot in memory dumps or debugger output.
 * 4. 32-bit value: Aligns well with common architectures.
 * 5. Non-zero: Helps detect uninitialized memory issues.
 * Using this value at the start and end of memory blocks helps identify
 * buffer overflows, underflows, and use-after-free bugs.
 */
#define BLOCK_MAGIC 0xDEADBEEF

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
  ssize_t _id;        /**< A random id. */
  size_t size;        /**< Size of the block of memory. */
  bool inuse;         /**< Flag indicating whether the block is free. */
  struct block *next; /**< Pointer to the next block in the heap. */
  struct block *prev; /**< Pointer to the previous block in the heap. */
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
  size_t total_size;   /**< Total size of the heap. */
  int64_t free_size;   /**< Size of the free memory in the heap. */
  size_t block_count;  /**< Number of blocks in the heap. */
  t_block *last_block; /**< Pointer to the last block in the heap. */
  t_block *blocks;     /**< Pointer to the first block in the heap. */
  struct heap *next;   /**< Pointer to the next heap. */
  struct heap *prev;   /**< Pointer to the previous heap. */
} t_heap;

// Instead of passing seperate integers. We use a struct to count the amount of
// blocks per block size
typedef struct s_amount {
  size_t tiny;
  size_t small;
  size_t large;
} t_amount;

// ****** Functions ****** //

__attribute__((warn_unused_result)) static inline size_t align(size_t n) {
  return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
}

__attribute__((warn_unused_result)) static inline size_t
determine_heap_size(size_t n, size_t amount_of_blocks) {
  if (n > (size_t)SMALL_HEAP_ALLOCATION_SIZE) {
    // Non-viable, creating exact size of user need
    return n + sizeof(t_heap) + (amount_of_blocks * sizeof(t_block));
  }

  if (n <= (size_t)TINY_BLOCK_SIZE - sizeof(t_block))
    return TINY_HEAP_ALLOCATION_SIZE;

  return SMALL_HEAP_ALLOCATION_SIZE;
}

__attribute__((warn_unused_result)) static inline size_t
determine_block_size(size_t n) {
  if (n <= (size_t)TINY_BLOCK_SIZE)
    return TINY_BLOCK_SIZE;

  return SMALL_BLOCK_SIZE;
}

__attribute__((warn_unused_result)) static inline struct s_amount
count_blocks(size_t n) {
  struct s_amount count = {0, 0, 0};

  if (n > (size_t)SMALL_HEAP_ALLOCATION_SIZE) {
    count.large++;
    return count;
  }

  while (n > 0) {
    if (n <= (size_t)TINY_BLOCK_SIZE - sizeof(t_block)) {
      count.tiny++;
      n = 0;
    } else if (n <= (size_t)SMALL_BLOCK_SIZE - sizeof(t_block)) {
      count.small++;
      n = 0;
    } else {
      count.small++;
      n = n - (SMALL_BLOCK_SIZE - sizeof(t_block));
    }
  }

  return count;
}

__attribute__((warn_unused_result)) static inline size_t
determine_total_block_size(size_t n) {
  size_t total_size = 0;
  int32_t remaining = n;
  int32_t TINY = TINY_BLOCK_SIZE - sizeof(t_block);
  int32_t SMALL = SMALL_BLOCK_SIZE - sizeof(t_block);

  while (remaining > 0) {
    if (remaining <= TINY) {
      remaining = remaining - TINY;
      total_size = total_size + TINY;
    } else {
      remaining = remaining - SMALL;
      total_size = total_size + SMALL;
    }
  }

  return total_size;
}

#endif // !ALLOC

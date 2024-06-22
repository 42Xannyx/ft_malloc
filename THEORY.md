# Theory

This document provides a brief explanation of the approach used in my custom memory allocator.

## My Allocator

### Data Structures

The following structs are used in the allocator:

```c
typedef struct block {
  size_t size;        /**< Size of the block of memory. */
  bool inuse;         /**< Flag indicating whether the block is free. */
  struct block *next; /**< Pointer to the next block in the heap. */
  struct block *prev; /**< Pointer to the previous block in the heap. */
} t_block;

typedef struct heap {
  size_t total_size;  /**< Total size of the heap. */
  size_t free_size;   /**< Size of the free memory in the heap. */
  size_t block_count; /**< Number of blocks in the heap. */
  t_block *blocks;    /**< Pointer to the first block in the heap. */
  struct heap *next;  /**< Pointer to the next heap. */
  struct heap *prev;  /**< Pointer to the previous heap. */
} t_heap;
```

### Explanation

The `t_heap` struct is obtained from `mmap()`. I initialize all the necessary values in the struct and create the blocks, which are then returned to the user who called the allocator.

### Block Sizes

There are three types of block sizes:

- **TINY**: Mallocs from 1 to `n` bytes, stored in `N` bytes big zones.
- **SMALL**: Mallocs from `(n+1)` to `m` bytes, stored in `M` bytes big zones.
- **LARGE**: Mallocs from `(m+1)` bytes and more, stored out of zone (with `mmap()`), each in its own zone.

### Size Definitions

- `TINY_HEAP_ALLOCATION_SIZE` = `4 * getpagesize()`
- `TINY_BLOCK_SIZE` = `TINY_HEAP_ALLOCATION_SIZE / 128`
- `SMALL_HEAP_ALLOCATION_SIZE` = `8 * getpagesize()`
- `SMALL_BLOCK_SIZE` = `SMALL_HEAP_ALLOCATION_SIZE / 128`
- `m` = `TINY_BLOCK_SIZE`
- `n` = `SMALL_BLOCK_SIZE`

You can find the page size by typing `getconf PAGE_SIZE` in your terminal, which is most commonly 4096.

### System Call Optimization

To minimize system calls like `mmap()`, I allocate a significant amount of memory at once when calling `mmap()`. The amount of memory allocated depends on the size of the first allocation:

- If the first allocation is TINY, `mmap()` allocates `TINY_HEAP_ALLOCATION_SIZE`.
- If the first allocation is SMALL, `mmap()` allocates `SMALL_HEAP_ALLOCATION_SIZE`.
- If the allocation size exceeds `SMALL_HEAP_ALLOCATION_SIZE`, the user is provided with their own heap, which is independent of the other heaps.

This strategy helps reduce the frequency of `mmap()` system calls by allocating larger chunks of memory at once, based on the initial allocation size.

If the `t_heap` is a size of `TINY_HEAP_ALLOCATION_SIZE` or `SMALL_HEAP_ALLOCATION_SIZE` it wwill be mixed with both sizes.

### A Block Pointer

There is an additional pointer that always points to the last block of the heap (`t_heap->last_block`). This way, I do not have to iterate through the entire heap each time; instead, I can get the last block via this pointer. If the `t_heap` is full, it will point to `NULL` until a block in the `t_heap` is free. This optimization significantly speeds up the program since it avoids unnecessary iterations through the `t_heap`.

### Performance Considerations

- **Reduced System Calls**: By allocating larger chunks of memory at once, the allocator minimizes the number of mmap() system calls, reducing overhead.
- **Efficient Block Management**: The use of a pointer to the last block (t_heap->last_block) reduces the need for iterating through the entire heap, speeding up allocation and deallocation operations.
- **Fragmentation Management**: The allocator coalesces adjacent free blocks to minimize fragmentation and make efficient use of available memory.

### Visual representation

![Untitled-2024-06-22-1416](https://github.com/42Xannyx/ft_malloc/assets/56308735/add8e84c-0621-4686-85ab-82befb0346a5)

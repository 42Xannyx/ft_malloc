#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "alloc.h"

#define BUFFER_SIZE 256

#if defined(DEBUG) || defined(TEST)
static void write_str(const char *str) {
  write(STDOUT_FILENO, str, strlen(str));
}

static void write_size_t(size_t value) {
  char buffer[BUFFER_SIZE];
  int len = snprintf(buffer, BUFFER_SIZE, "%zu", value);
  write(STDOUT_FILENO, buffer, len);
}

static void write_int(int64_t value) {
  char buffer[BUFFER_SIZE];
  int len = snprintf(buffer, BUFFER_SIZE, "%lld", value);
  write(STDOUT_FILENO, buffer, len);
}

static void write_ptr(void *ptr) {
  char buffer[BUFFER_SIZE];
  int len = snprintf(buffer, BUFFER_SIZE, "%p", ptr);
  write(STDOUT_FILENO, buffer, len);
}

static void write_bool(bool value) { write_str(value ? "Yes" : "No"); }

void print_sizes(void) {
  write_str("\033[1;34mTINY_HEAP_ALLOCATION_SIZE\033[0m: ");
  write_size_t(TINY_HEAP_ALLOCATION_SIZE);
  write_str(" bytes\n");

  write_str("\033[1;34mTINY_BLOCK_SIZE\033[0m: ");
  write_size_t(TINY_BLOCK_SIZE);
  write_str(" bytes\n");

  write_str("\033[1;32mSMALL_HEAP_ALLOCATION_SIZE\033[0m: ");
  write_size_t(SMALL_HEAP_ALLOCATION_SIZE);
  write_str(" bytes\n");

  write_str("\033[1;32mSMALL_BLOCK_SIZE\033[0m: ");
  write_size_t(SMALL_BLOCK_SIZE);
  write_str(" bytes\n");
}

void print_block_count(t_amount block_count) {
  printf("\n--- Block Allocation Summary ---\n");

  printf("\033[1;34mTINY Blocks\033[0m:\n");
  printf("  Size: %d bytes\n", TINY_BLOCK_SIZE);
  printf("  Count: %zu\n", block_count.tiny);
  printf("  Total: %zu bytes\n", block_count.tiny * TINY_BLOCK_SIZE);

  printf("\033[1;32mSMALL Blocks\033[0m:\n");
  printf("  Size: %d bytes\n", SMALL_BLOCK_SIZE);
  printf("  Count: %zu\n", block_count.small);
  printf("  Total: %zu bytes\n", block_count.small * SMALL_BLOCK_SIZE);

  printf("\nAllocation Sizes:\n");
  printf("  \033[1;34mTINY_HEAP_ALLOCATION_SIZE\033[0m: %d bytes\n",
         TINY_HEAP_ALLOCATION_SIZE);
  printf("  \033[1;32mSMALL_HEAP_ALLOCATION_SIZE\033[0m: %d bytes\n",
         SMALL_HEAP_ALLOCATION_SIZE);

  printf("\nTotal Blocks: %zu\n", block_count.tiny + block_count.small);
  printf("Total Allocated: %zu bytes\n",
         (block_count.tiny * TINY_BLOCK_SIZE) +
             (block_count.small * SMALL_BLOCK_SIZE));

  printf("--- End of Summary ---\n");
}

void print_block(t_block *block) {
  write_str("\033[1;33mBlock Address\033[0m: ");
  write_ptr((void *)block);
  write_str("\n");

  write_str("  \033[1;36mBlock id\033[0m: ");
  write_size_t(block->_id);
  write_str("\n");

  write_str("  \033[1;36mSize (exc canary)\033[0m: ");
  write_size_t(block->size - 1);
  write_str(" bytes\n");

  write_str("  \033[1;36mSize with metadata\033[0m: ");
  write_size_t(block->size + sizeof(t_block));
  write_str(" bytes\n");

  write_str("  \033[1;36mIn Use\033[0m: ");
  write_bool(block->inuse);
  write_str("\n");

  write_str("  \033[1;36mNext Block\033[0m: ");
  write_ptr((void *)block->next);
  write_str("\n");

  write_str("  \033[1;36mPrevious Block\033[0m: ");
  write_ptr((void *)block->prev);
  write_str("\n");
}

void print_heap(t_heap *heap, bool show_blocks) {
  write_str("\033[1;35mHeap Address\033[0m: ");
  write_ptr((void *)heap);
  write_str("\n");

  write_str("  \033[1;36mTotal Size (inc. metadata)\033[0m: ");
  write_size_t(heap->total_size);
  write_str(" bytes\n");

  write_str("  \033[1;36mFree Size\033[0m: ");
  write_int(heap->free_size);
  write_str(" bytes\n");

  write_str("  \033[1;36mBlock Count\033[0m: ");
  write_size_t(heap->block_count);
  write_str("\n");

  write_str("  \033[1;36mNext Heap\033[0m: ");
  write_ptr((void *)heap->next);
  write_str("\n");

  write_str("  \033[1;36mPrevious Heap\033[0m: ");
  write_ptr((void *)heap->prev);
  write_str("\n");

  if (heap->last_block) {
    write_str("  \033[1;36mLast Block:\033[0m\n");
    print_block(heap->last_block);
  }

  if (show_blocks) {
    write_str("  \n\033[1;36mBlocks:\033[0m\n");
    t_block *current_block = heap->blocks;
    while (current_block != NULL) {
      print_block(current_block);
      current_block = current_block->next;
    }
  }
}
#endif // DEBUG

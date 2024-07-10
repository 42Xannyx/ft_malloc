#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "alloc.h"

#define STDOUT_FILENO 1
#define BUFFER_SIZE 256

#ifdef DEBUG
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
  int len = snprintf(buffer, BUFFER_SIZE, "%ld", value);
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

void print_block(t_block *block) {
  write_str("\033[1;33mMAGIC START\033[0m: ");
  write_size_t(block->magic_start);
  write_str("\n");

  write_str("\033[1;33m------------------\033[0m\n");

  write_str("\033[1;33mBlock Address\033[0m: ");
  write_ptr((void *)block);
  write_str("\n");

  write_str("  \033[1;36mSize\033[0m: ");
  write_size_t(block->size);
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

  write_str("\033[1;33m------------------\033[0m\n");

  write_str("\033[1;33mMAGIC END\033[0m: ");
  write_size_t(block->magic_end);
  write_str("\n");
}

void print_heap(t_heap *heap, bool show_blocks) {
  write_str("\033[1;35mHeap Address\033[0m: ");
  write_ptr((void *)heap);
  write_str("\n");

  write_str("  \033[1;36mTotal Size\033[0m: ");
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

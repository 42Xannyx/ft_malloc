#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined(__APPLE__)
#include <malloc/malloc.h>
#elif defined(__linux__)
#include <malloc.h>
#endif

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

#define LARGE_ALLOC 32896
#define BUFFER_SIZE 10000

#define DEBUG 1

char buffer[10000];

void fill_string(char *ptr, int64_t n) {
  for (int i = 0; i < n; i++) {
    ptr[i] = (i % 26) + 'A';
  }
  ptr[n] = '\0';
}

// Function to capture stdout
char *capture_stdout(void (*func)(void)) {
  // Redirect stdout to a pipe
  int pipe_fd[2];
  if (pipe(pipe_fd) != 0) {
    return NULL;
  }

  int old_stdout = dup(STDOUT_FILENO);
  dup2(pipe_fd[1], STDOUT_FILENO);
  close(pipe_fd[1]);

  // Run the function
  func();

  // Restore stdout
  fflush(stdout);
  dup2(old_stdout, STDOUT_FILENO);
  close(old_stdout);

  // Read the captured output
  ssize_t bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE - 1);
  close(pipe_fd[0]);

  if (bytes_read >= 0) {
    buffer[bytes_read] = '\0'; // Null-terminate the string
  } else {
    return NULL;
  }

  return buffer;
}

// Function to check if a string contains a substring
int contains_substring(const char *str, const char *substr) {
  return strstr(str, substr) != NULL;
}

// Function to extract mmap size
size_t extract_mmap_size(const char *output) {
  const char *mmap_str = strstr(output, "Calling mmap with total_size ");
  if (mmap_str) {
    return (size_t)atoi(mmap_str + 29);
  }
  return 0;
}

// Test functions (modified to return void for use with capture_stdout)
void test_large_allocation(void) {
  char *test = ft_malloc(sizeof(char) * LARGE_ALLOC);

  assert(test != NULL);
  fill_string(test, 10);
  ft_free(test);
  g_heap = NULL;
}

void test_tiny_allocation(void) {
  char *test = ft_malloc(sizeof(char) * 64);
  if (test) {
    fill_string(test, 10);
    ft_free(test);
  }
}

void test_single_byte_allocation(void) {
  printf("Test 4: Single byte allocation\n");
  char *test = ft_malloc(sizeof(char));
  if (test) {
    fill_string(test, 1);
    ft_free(test);
  }
}

void test_zero_allocation(void) {
  printf("Test 5: Zero allocation\n");
  void *ptr = ft_malloc(0);
  assert(ptr == NULL);
}

void test_multiple_allocations(void) {
  printf("Test 6: Multiple allocations\n");
  char *test1 = ft_malloc(sizeof(char) * LARGE_ALLOC);
  char *test2 = ft_malloc(sizeof(char) * LARGE_ALLOC);
  if (test1 && test2) {
    ft_free(test1);
    ft_free(test2);
  }
}

int main(void) {
  char *output;

  // Test 1: Large allocation
  output = capture_stdout(test_large_allocation);
  assert(contains_substring(output, "Calling mmap with total_size 43968"));
  assert(contains_substring(output, "Call munmap()"));
  assert(contains_substring(output, "LARGE allocation"));
  assert(contains_substring(output, "Block Count\033[0m: 172"));
  assert(extract_mmap_size(output) == 43968);
#if DEBUG
  printf("Test 1: Large allocation\n");
  printf("--- Test 1 ---\n%s\n------- END TEST 1 --------\n", output);
  fflush(stdout);
#endif

  // Test 2: Another Large allocation
  output = capture_stdout(test_large_allocation);
  assert(contains_substring(output, "Calling mmap with total_size 43968"));
  assert(contains_substring(output, "Call munmap()"));
  assert(contains_substring(output, "LARGE allocation"));
  assert(contains_substring(output, "Block Count\033[0m: 172"));
  assert(extract_mmap_size(output) == 43968);
  assert(contains_substring(output, "Previous Heap\033[0m: 0x0"));
#if DEBUG
  printf("Test 2: Another Large allocation\n");
  printf("--- Test 2 ---\n%s\n------- END TEST 2 --------\n", output);
  fflush(stdout);
#endif

  // Test 3: Tiny allocation
  output = capture_stdout(test_tiny_allocation);
  assert(contains_substring(output, "Calling mmap"));
  assert(contains_substring(output, "Call munmap()"));
  assert(contains_substring(output, "Block Count\033[0m: 1"));
  assert(extract_mmap_size(output) == (size_t)TINY_HEAP_ALLOCATION_SIZE);
#if DEBUG
  printf("Test 3: Tiny allocation\n");
  printf("--- Test 3 ---\n%s\n------- END TEST 3 --------\n", output);
  fflush(stdout);
#endif

  // Test 4: Single byte allocation
  output = capture_stdout(test_single_byte_allocation);
  assert(contains_substring(output, "Calling mmap"));
  assert(contains_substring(output, "Call munmap()"));
  /*assert(extract_mmap_size(output) == 192);*/

  /*#if DEBUG*/
  /*  printf("--- Test 4 Output ---\n%s\n----------------------\n", output);*/
  /*#endif*/
  /**/
  /*  // Test 5: Zero allocation*/
  /*  assert(ft_malloc(0) == NULL);*/
  /*#if DEBUG*/
  /*  printf("--- Test 5 Output ---\n%s\n----------------------\n", output);*/
  /*#endif*/
  /**/
  /*  // Test 6: Multiple allocations*/
  /*  output = capture_stdout(test_multiple_allocations);*/
  /*  assert(contains_substring(output, "Calling mmap"));*/
  /*  assert(contains_substring(output, "Call munmap()"));*/
  /*  // Check for two mmap calls*/
  /*  char *second_mmap = strstr(output, "Calling mmap") + 1;*/
  /*  assert(second_mmap && strstr(second_mmap, "Calling mmap"));*/
  /*#if DEBUG*/
  /*  printf("--- Test 6 Output ---\n%s\n----------------------\n", output);*/
  /*#endif*/

  printf("All tests completed successfully.\n");
  return 0;
}

#include <malloc/malloc.h>
#include <stdint.h>
#include <stdio.h>

#include "libft_plus.h"

int32_t main() {
  // ft_malloc(0);
  char *test1 = malloc(sizeof(char) * 0);

  for (int i = 0; i < 20; i++) {
    test1[i] = 'A'; // Fill with 'A'
  }
  test1[20] = '\0'; // Null-terminator
  printf("Malloc test1: %s\n", test1);

  size_t size = malloc_size(test1);
  printf("Allocated memory size: %zu bytes\n", size);

  return 0;
}

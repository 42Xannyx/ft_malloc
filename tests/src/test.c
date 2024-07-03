#ifdef __APPLE__
#include <malloc/malloc.h>
#endif

#ifdef linux
#include <malloc.h>
#endif

#include "libft_plus.h"

#include <assert.h>
#include <string.h>

void stress_test(void) {
  for (int i = 0; i < 86; i++) {
    ft_malloc(sizeof(char) * 128);
  }
}

void fill_string(char *ptr, int64_t n) {
  for (int i = 0; i < n; i++) {
    ptr[i] = (i % 26) + 'A';
  }

  ptr[n] = '\0';
}

int32_t main(void) {
  assert(ft_malloc(0) == NULL);

  /*char *another_ptr = ft_malloc(sizeof(char) * 510);*/
  /*char *anot = ft_malloc(sizeof(char) * 500);*/

  /*(void)anot;*/

  stress_test();

  /*fill_string(ptr, 10);*/
  /*fill_string(another_ptr, 500);*/

  /*printf("Malloc: %s\n", ptr);*/
  /*printf("strlen: %ld\n", strlen(ptr));*/

  /*printf("Malloc: %s\n", another_ptr);*/
  /*printf("strlen: %ld\n", strlen(another_ptr));*/

  return 0;
}

#include "alloc.h"
#include <malloc/_platform.h>
#include <stdio.h>
#ifdef __APPLE__
#include <malloc/malloc.h>
#endif

#ifdef linux
#include <malloc.h>
#endif

#include "debug.h"
#include "libft_plus.h"

#include <assert.h>
#include <string.h>

size_t size_heap() {
  t_heap *tmp = g_heap;

  if (!tmp) {
    return 0;
  }

  size_t len = 0;
  while (tmp) {
    len++;
    tmp = tmp->prev;
  }

  return len;
}

void stress_test(void) {
  for (int i = 0; i < 128; i++) {
    ft_malloc(sizeof(char) * 1);
  }
}

void fill_string(char *ptr, int64_t n) {
  for (int i = 0; i < n; i++) {
    ptr[i] = (i % 26) + 'A';
  }

  ptr[n] = '\0';
}

int32_t main(void) {

  /* Test 1
   * Should return a ptr with a total of 172 blocks.
   * 171 SMALL Blocks & 1 TINY Block
   *
   * It should create a completly new heap, seperated from the others.
   *
   * It should actually allocate: amount_of_user + sizeof(heap_struct) +
   * (amount_of_blocks * sizeof(t_block))
   *
   * So by the time of writing:
   * 32896 + 64 + (172 * 64) = 43968
   */
  char *test_1 = ft_malloc(sizeof(char) * 32896);
  fill_string(test_1, 10);

#ifdef DEBUG
  print_block((t_block *)test_1);
#endif

  ft_free(test_1);
  test_1 = NULL;

  /* END TEST 1 */

  /* Test 2
   * Same as last run, but shows that the last heap is gone, because of ft_free.
   * Check the address of the two heaps to make sure it is different
   */
  char *test_2 = ft_malloc(sizeof(char) * 32896);
  fill_string(test_2, 10);

#ifdef DEBUG
  print_block((t_block *)test_2);
#endif

  ft_free(test_2);
  test_2 = NULL;

  /* END TEST 2 */

  /* Test 3
   * Create a block which is exactly one TINY block
   */
  char *test_3 = ft_malloc(sizeof(char) * 64);
  fill_string(test_3, 10);

#ifdef DEBUG
  print_block((t_block *)test_3);
#endif

  ft_free(test_3);
  test_3 = NULL;

  /* END TEST 3 */

  /* Test 4
   * Create a TINY blocks which has 64 bytes of free space
   */
  char *test_4 = ft_malloc(sizeof(char) * 1);
  fill_string(test_4, 1);

#ifdef DEBUG
  print_block((t_block *)test_4);
#endif

  ft_free(test_4);
  test_4 = NULL;

  /* END TEST 4 */

  /* Test 5
   * A size of 0, which should immediatly return NULL
   */

  assert(ft_malloc(0) == NULL);

  /* END TEST 5 */

  /* Test 6
   */
  char *test_6_1 = ft_malloc(sizeof(char) * 32896);
  char *test_6_2 = ft_malloc(sizeof(char) * 32896);

  assert(size_heap() == 2);

  ft_free(test_6_1);
  ft_free(test_6_2);
  test_2 = NULL;

  /* END TEST 6 */

  return 0;
}

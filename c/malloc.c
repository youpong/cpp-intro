#include "util.h"
#include <stdlib.h>

/* If size is  0,  then malloc() returns either NULL, or a unique pointer
   value that can later be successfully passed to free(). */
static void test_malloc_size0() {
  void *p = malloc(0);
  free(p);
}

/* If ptr is NULL, no operation is performed. */
static void test_free_NULL() {
  free(NULL);
  free(NULL);
}

void test_all_malloc() {
  test_malloc_size0();
  test_free_NULL();
}

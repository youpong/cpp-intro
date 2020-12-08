#include "util.h"
#include <stdlib.h>

void test_all_memcpy();

extern int gl_v;

/*
 * compile below type to golang
 * ----------------------------
 *
 * 	int (* (*ptr0)(int (*)(int)))(int);
 *
 * s1. analyse outside of (* ...)
 * = int (* ...)(int)
 * -> func(int) int
 * s2. analyse inside of (* ...)
 * = * (*ptr0)(int (*)(int))
 *   s2.1. analyse outside of (*ptr0)
 *   -> func(param) * ___
 *     s2.1.1. analyse param
 *     param = int (*)(int)
 *       s2.1.1.1 analyse outside of (*)
 *       -> func(int) int
 *       s2.1.1.2 analyse inside of (*)
 *       -> * ___
 *       s2.1.1.3 combine
 *       -> * func(int) int
 *     s2.1.2. substitute param
 *     -> func( * func(int) int ) * ___
 *   s2.2. analyse inside of (*ptr0)
 *   = *ptr0
 *   -> * ___
 *   s2.3. combine
 *   -> * func( * func(int) int ) * ___
 *    s3. combine
 * -> * func( * func(int) int ) * func(int) int
 */

typedef int (*f_ptr)(int);

static int f(int x) {
  ++gl_v;
  return x;
}

static f_ptr g(f_ptr p) {
  p(0);
  return p;
}

static void test_function_ptr() {
  // form1
  int (*(*ptr0)(int (*)(int)))(int) = &g;

  // form2: typedef
  f_ptr (*ptr1)(f_ptr) = &g;

  (*ptr0)(&f);
  ptr0(&f);
  ptr1(&f);
  expect(__LINE__, 3, gl_v);
}

static void test_array_ptr() {
  int a[5] = {1, 2, 3, 4, 5};

  int(*ptr)[5] = &a; // ptr is a pointer to array of int, len 5.

  expect(__LINE__, 2, (*ptr)[1]); // !=  *ptr[1]
}

int gl_v = 0;

int main(int argc, char **argv) {
  test_all_memcpy();
  test_function_ptr();
  test_array_ptr();
  return EXIT_SUCCESS;
}

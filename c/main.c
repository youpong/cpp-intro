#include "util.h"
#include <stdlib.h>

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

int gl_v = 0;

int main(int argc, char **argv) {
  test_function_ptr();
  return EXIT_SUCCESS;
}

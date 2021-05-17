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

static void test_array_ptr() {
  int a[] = {1, 2, 3, 4, 5}; // a is an array of int, len 5.

  {
    int(*ptr)[5] = &a; // ptr is a pointer to array of int, len 5.

    expect(__LINE__, 2, (*ptr)[1]); // !=  *ptr[1]
    expect(__LINE__, 5, sizeof(*ptr) / sizeof(int));
  }

  {
      /*
       *  gcc:
       * warning: initialization of ‘int (*)[4]’ from incompatible
       * pointer type ‘int (*)[5]’ [-Wincompatible-pointer-types]
       *
       * clang:
       * warning: incompatible pointer types initializing 'int (*)[4]'
       * with an expression of type 'int (*)[5]'
       * [-Wincompatible-pointer-types]
       *
       *            int (*ptr)[4] = &a;
       */
  }

  {
    // type conversion through casting void pointer
    int(*ptr)[4] = (void *)&a;
    expect(__LINE__, 16, sizeof(*ptr));
  }

  {
    // type conversion
    int(*ptr)[4] = (int(*)[4]) & a;

    expect(__LINE__, 16, sizeof(*ptr));
  }
}

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

static void test_array_length() {
  {
    int a[] = {0, 1, 2, 3, 4};
    expect(__LINE__, 5, ARRAY_LENGTH(a));
  }

  {
    int a[] = {};
    expect(__LINE__, 0, ARRAY_LENGTH(a));
  }
}

void test_all_memcpy();
void test_all_malloc();
void test_all_setjmp();

int gl_v = 0;

int main(int argc, char **argv) {
  test_all_malloc();
  test_all_memcpy();
  test_all_setjmp();
  
  test_function_ptr();
  test_array_ptr();
  test_array_length();

  return EXIT_SUCCESS;
}

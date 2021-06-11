#include "util.h"

// 1. not function pointer
typedef int number;

// 2. function pointer
typedef int (*f_ptr)(int);

static number func(number n) { return n + 1; }

static int func1(int n) { return n + 2; }

static void test_func() {
  expect(__LINE__, 2, func(1));

  f_ptr ptr = &func1;

  expect(__LINE__, 3, (*ptr)(1));
}

void test_all_typedef() { test_func(); }

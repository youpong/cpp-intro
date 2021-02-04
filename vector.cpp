#include "main.h"

static void test_allocator() {
  std::allocator<std::string> a;

#ifndef QUIET_MODE
  // warning: ignoring return value of function declared with 'nodiscard'
  a.allocate(1);
#endif

  std::string *p = a.allocate(1);

  std::string *s = new (p) std::string("hello");
  expect(__LINE__, "hello"s, *s);

  s->~basic_string();

  a.deallocate(s, 1);
}

void test_all_vector() {
  test_allocator();
  expect(__LINE__, 0, 0);
}

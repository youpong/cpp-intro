#include "main.h"

static void test_allocator() {
  std::allocator<std::string> a;
  using traits = std::allocator_traits<decltype(a)>;

#ifndef QUIET_MODE
  // warning: ignoring return value of function declared with 'nodiscard'
  a.allocate(1);
#endif

  std::string *p = traits::allocate(a, 1);

  // vector.cpp:14:16: error: cannot initialize a variable of type
  // 'std::string *' (aka 'basic_string<char> *') with an rvalue
  // of type 'void'
  std::string *s = traits::construct(a, p, "hello");
  expect(__LINE__, "hello"s, *s);

  traits::destroy(a, s);

  traits::deallocate(a, p, 1);
}

void test_all_vector() {
  test_allocator();
  expect(__LINE__, 0, 0);
}

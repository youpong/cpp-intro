#include "main.h"

template <class T>
class allocator {};

static void test_allocate_size0() {
  //  expect(__LINE__, 0, 1);
  std::allocator<int> i;
  int *p = i.allocate(0);
  i.deallocate(p, 0);
  // i.deallocate(p, 0); valid if p is not NULL
}

static void test_deallocate_size0() {
  //  expect(__LINE__, 0, 1);
  std::allocator<int> i;
  i.deallocate(nullptr, 0);
}

static void test_allocator() {
  std::allocator<std::string> a;
  using traits = std::allocator_traits<decltype(a)>;

#ifndef QUIET_MODE
  // warning: ignoring return value of function declared with 'nodiscard'
  a.allocate(1);
#endif

  std::string *p = traits::allocate(a, 1);

#ifdef ISSUE_4
  // error: cannot initialize a variable of type 'std::string *'
  // (aka 'basic_string<char> *') with an rvalue of type 'void'
  std::string *s = traits::construct(a, p, "hello");
#else
  std::string *s = new (p) std::string("hello");
#endif
  expect(__LINE__, "hello"s, *s);

  traits::destroy(a, s);

  traits::deallocate(a, p, 1);
}

static void test_allocators(std::size_t n) {
  std::allocator<std::string> a;
  using traits = std::allocator_traits<decltype(a)>;

  std::string *p = traits::allocate(a, n);

  for (std::size_t i = 0; i != n; ++i) {
    std::string *s = new (p + i) std::string("hello");
    expect(__LINE__, "hello"s, *s);
    traits::destroy(a, s);
  }

  traits::deallocate(a, p, n);
}

/*
static void test_allocate() {
  std::allocator<std::string> a;

  allocate(1);
}
*/

void test_all_allocator() {
  test_allocate_size0();
  test_deallocate_size0();
  test_allocator();
  test_allocators(0);
  test_allocators(5);
  //  test_allocate();
  //  expect(__LINE__, 0, );
}

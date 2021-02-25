#include "main.h"

namespace ns {
template <class T>
class allocator {
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;
  using is_always_equal = std::true_type;

  constexpr allocator() noexcept;
  constexpr allocator(const allocator &) noexcept;
  template <class U>
  constexpr allocator(const allocator<U> &) noexcept;
  ~allocator();
  allocator &operator=(const allocator &) = default;
  [[nodiscard]] T *allocate(std::size_t n);
  void deallocate(T *p, std::size_t n);
};

template <typename T>
void destroy_at(T *location) {
  location->~T();
}
} // end of namespace ns

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
  std::string *p = a.allocate(1);
  std::string *s = new (p) std::string("Hello");

  // error: identifier 'string' in object destruction expression does not
  // name a type
  // s->~string();
  s->~basic_string();

  a.deallocate(p, 1);
}

static void test_destroy_at(bool use_std) {
  std::allocator<std::string> a;
  std::string *p = a.allocate(1);
  std::string *s = new (p) std::string("Hello");
  if (use_std)
    std::destroy_at(s);
  else
    ns::destroy_at(s);

  a.deallocate(p, 1);
}

static void test_allocator_traits() {
  std::allocator<std::string> a;
  using traits = std::allocator_traits<decltype(a)>;

#ifndef QUIET_MODE
  // warning: ignoring return value of function declared with 'nodiscard'
  a.allocate(1);
#endif

  std::string *p = traits::allocate(a, 1);

  traits::construct(a, p, "hello");
  expect(__LINE__, "hello"s, *p);

  traits::destroy(a, p);

  traits::deallocate(a, p, 1);
}

static void test_allocators(std::size_t n) {
  std::allocator<std::string> a;
  using traits = std::allocator_traits<decltype(a)>;

  std::string *p = traits::allocate(a, n);

  for (auto i = p, last = p + n; i != last; ++i) {
    traits::construct(a, i, "hello");
    expect(__LINE__, "hello"s, *i);
  }

  for (auto i = p, last = p + n; i != last; ++i) {
    traits::destroy(a, i);
  }

  traits::deallocate(a, p, n);
}

void test_all_allocator() {
  test_allocate_size0();
  test_deallocate_size0();
  test_allocator();
  test_destroy_at(true);
  test_destroy_at(false);
  test_allocator_traits();
  test_allocators(0);
  test_allocators(5);
}

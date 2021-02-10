#include "main.h"

template <typename T, typename Allocator = std::allocator<T>>
class vector {
public:
  // clang-format off
  using value_type      = T;
  using pointer         = T *;
  using const_pointer   = const pointer;
  using reference       = value_type &;
  using const_reference = const value_type &;
  using allocator_type  = Allocator;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;

  using iterator               = pointer;
  using const_iterator         = const_pointer;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  // clang-format on
private:
  /*
  pointer first = nullptr;
  size_type valid_size = 0; // original = nullptr;
  size_type allocated_size = 0; // original = nullptr;
  allocator_type alloc;
  */
  pointer first = nullptr;
  pointer last = nullptr;
  pointer reserved_last = nullptr;
  allocator_type alloc;

public:
  vector(std::size_t n = 0, Allocator a = Allocator());
  ~vector();
  vector(const vector &x);
  vector &operator=(const vector &x);

  void push_back(const T &x);
  T &operator[](std::size_t i) noexcept;

  iterator begin() noexcept { return first; }
  iterator end() noexcept { return last; }
  iterator begin() const noexcept { return first; }
  iterator end() const noexcept { return last; }
  const_iterator cbegin() const noexcept { return first; }
  const_iterator cend() const noexcept { return last; }
};

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

static void test_vector() {
  // vector<int> v(100);
  std::vector<int> v(100);
  for (auto i = 0; i != 100; ++i)
    v[i] = i;

  return;
  std::for_each(std::begin(v), std::end(v),
                [](auto x) { std::cout << x; });

  std::copy(std::istream_iterator<int>(std::cin),
            std::istream_iterator<int>(), std::back_inserter(v));
}

static void test_nested_typename() {
  using vec = std::vector<int>;
  // using vec = vector<int>;
  vec v = {0, 1, 2};

  vec::value_type val = v[1];
  expect(__LINE__, 1, val);
  vec::iterator i = v.begin();
  expect(__LINE__, 0, *i);
  vec::size_type s = v.size();
  expect(__LINE__, 3, s);
  auto j = i + 3;
  vec::difference_type d = j - i;
  expect(__LINE__, 3, d);
}

/*
static void test_foo() {
  //vector<int> vec = { 0, 1, 2 };
  vector<int>::size_type s = 1;
}
*/

static void test_foo() {
  std::vector<int> v(1);
  // std::vector<int>::iterator
  auto i = v.begin();
  *i = 0;

  auto const &cv = v;
  // std::vector<int>::const_iterator
  [[maybe_unused]] auto ci = cv.begin();
  // error
  // can't assign pointed by const_iterator
  //  *ci = 0;
}

void test_all_vector() {
  test_allocator();
  test_allocators(0);
  test_allocators(5);
  test_vector();
  test_nested_typename();
  test_foo();
}

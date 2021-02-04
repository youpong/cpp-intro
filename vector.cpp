#include "main.h"

template < typename T, typename Allocator = std::allocator<T> >
class vector {
private :
public:
  using value_type = T;
  // TODO:
  using iterator = T *;
  
  vector( std::size_t n = 0, Allocator a = Allocator() );
  ~vector() ;
  vector( const vector & x );
  vector & operator =(const vector & x);

  void push_back( const T & x );
  T & operator [](std::size_t i ) noexcept;

  iterator begin() noexcept;
  iterator end() noexcept;
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

void test_all_vector() {
  test_allocator();
  test_allocators(0);
  test_allocators(5);
}

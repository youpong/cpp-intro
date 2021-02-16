#include "main.h"

// support std::scope_exit()
// #define SCOPE_EXIT

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
  using traits = std::allocator_traits<allocator_type>;

  pointer first = nullptr;
  pointer last = nullptr;
  pointer reserved_last = nullptr;
  allocator_type alloc;

  //
  // allocate/deallocate
  //

  // allocates n bytes and returns a pointer to the allocated memory.
  pointer allocate(size_type n) { return traits::allocate(alloc, n); }
  // free pointer first.
  void deallocate() { traits::deallocate(alloc, first, capacity()); }

  //
  // construct/destroy
  //

  void construct(pointer ptr) { traits::construct(alloc, ptr); }
  void construct(pointer ptr, const_reference value) {
    traits::construct(alloc, ptr, value);
  }
  void construct(pointer ptr, value_type &&value) {
    traits::construct(alloc, ptr, std::move(value));
  }
  void destroy(pointer ptr) { traits::destroy(alloc, ptr); }
  void destroy_until(reverse_iterator rend) {
    for (auto riter = rbegin(); riter != rend; ++riter, --last) {
      destroy(&*riter);
    }
  }
  void clear() noexcept { destroy_until(rend()); }

public:
  //
  // constructors ...
  //

  //  vector(std::size_t n = 0, Allocator a = Allocator()); original
  vector(size_type size = 0,
         const allocator_type &alloc = allocator_type())
      : alloc(alloc) {
    resize(size);
  }
  vector(size_type size, const_reference value,
         const allocator_type &alloc = allocator_type())
      : alloc(alloc) {
    resize(size, value);
  }

  template <typename InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type &alloc = allocator_type())
      : alloc(alloc) {
    reserve(std::distance(first, last));
    for (auto i = first; i != last; ++i) {
      push_back(*i);
    }
  }

  vector(std::initializer_list<value_type> init,
         const allocator_type &alloc = allocator_type())
      : vector(std::begin(init), std::end(init), alloc) {}

  vector(const vector &x);

  ~vector() {
    clear();
    deallocate();
  }

  void reserve(size_type sz) {
    if (sz <= capacity())
      return;

    auto ptr = allocate(sz);

    auto old_first = first;
    auto old_last = last;
    auto old_capacity = capacity();

    first = ptr;
    last = first;
    reserved_last = first + sz;
#ifdef SCOPE_EXIT
    std::scope_exit e(
        [&] { traits::deallocate(alloc, old_first, old_capacity); });
#endif
    for (auto old_iter = old_first; old_iter != old_last;
         ++old_iter, ++last) {
      construct(last, std::move(*old_iter));
    }

    auto riter = reverse_iterator(old_last);
    auto rend = reverse_iterator(old_first);
    while (riter != rend) {
      destroy(&*riter);
      ++riter;
    }

#ifndef SCOPE_EXIT
    traits::deallocate(alloc, old_first, old_capacity);
#endif
  }

  void resize(size_type sz) {
    if (sz < size()) {
      auto diff = size() - sz;
      destroy_until(rbegin() + diff);
      last = first + sz;
    } else if (sz > size()) {
      reserve(sz);
      for (; last != reserved_last; ++last) {
        construct(last);
      }
    }
  }

  void resize(size_type sz, const_reference value) {
    if (sz < size()) {
      auto diff = size() - sz;
      destroy_until(rbegin() + diff);
      last = first + sz;
    } else if (sz > size()) {
      reserve(sz);
      for (; last != reserved_last; ++last) {
        construct(last, value);
      }
    }
  }

  void shrink_to_fit() {
    if (size() == capacity())
      return;

    auto current_size = size();
    auto ptr = allocate(current_size);

    for (auto raw_ptr = ptr, iter = begin(); iter != end();
         ++iter, ++raw_ptr) {
      construct(raw_ptr, *iter);
    }

    clear();
    deallocate();

    first = ptr;
    last = ptr + current_size;
    reserved_last = last;
  }

  vector &operator=(const vector &x);

  void push_back(const_reference value) {
    if (size() + 1 > capacity()) {
      reserve(capacity() + 10);
    }
    construct(last, value);
    ++last;
  }

  reference operator[](size_type i) { return first[i]; }
  const_reference operator[](size_type i) const { return first[i]; }
  reference at(size_type i) {
    if (i >= size())
      throw std::out_of_range("index is out of range");
    return first[i];
  }
  const_reference at(size_type i) const {
    if (i >= size())
      throw std::out_of_range("index is out of range");
    return first[i];
  }
  reference front() { return first; }
  const_reference front() const { return first; }
  reference back() { return last - 1; }
  const_reference back() const { return last - 1; }
  pointer data() noexcept { return first; }
  const_pointer data() const noexcept { return first; }

  // clang-format off
  iterator       begin()        noexcept { return first; }
  iterator       end()          noexcept { return last;  }
  iterator       begin()  const noexcept { return first; }
  iterator       end()    const noexcept { return last;  }
  const_iterator cbegin() const noexcept { return first; }
  const_iterator cend()   const noexcept { return last;  }
  
  reverse_iterator       rbegin()        noexcept {
    return reverse_iterator{last};
  }
  reverse_iterator       rend()          noexcept {
    return reverse_iterator{first};
  }
  const_reverse_iterator crbegin() const noexcept {
    return reverse_iterator{last};
  }
  const_reverse_iterator crend()   const noexcept {
    return reverse_iterator{first};
  }
  // clang-format on

  size_type size() const noexcept {
    //    return end() - begin();
    return std::distance(begin(), end());
  }

  size_type empty() const noexcept {
    //    return begin() == end();
    return size() == 0;
  }

  size_type capacity() const noexcept { return reserved_last - first; }
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
/*
template <typename Vector>
static void test_bar() {
  Vector v(100);
}
*/

template <typename Vector>
static void test_vector() {
  Vector v(100);
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
  //  vector<int> v(1);

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

static void test_iterator() {
  std::vector<int> v = {0, 1, 2, 3, 4};
  // rbegin()
  // check reverse iterator last -> first
  // manipulate value via iterator

  // crbegin()
  // check reverse iterator last -> first
  // cannot manipulate value via const_reverse_iterator
}

static void test_size() {
  // vector<int> v2(0);
  // vector<int, std::allocator<int>> v2(0);
  // std::vector<int> v(0);
  // std::vector<int, std::allocator<int>> v(0);
}

static void test_vector2() {
  vector<int> v;
  expect(__LINE__, 0, v.capacity());
  expect(__LINE__, 0, v.size());

  v.push_back(0);
  expect(__LINE__, 0, v[0]);
  v.push_back(1);
  expect(__LINE__, 1, v[1]);
  expect(__LINE__, 2, v.size());
}

template <typename Vector>
static void test_reserve() {
  Vector v;
  v.reserve(0);
}

void test_all_vector() {
  test_reserve<std::vector<int>>();
  test_reserve<vector<int>>();
  test_vector2();
  test_size();
  test_iterator();
  test_allocator();
  test_allocators(0);
  test_allocators(5);
  test_vector<std::vector<int>>();
  test_nested_typename();
  test_foo();
}

/*
template <typename T, typename Allocator = std::allocator<T>>
class S {
  using allocator_type = Allocator;
  using traits = std::allocator_traits<allocator_type>;
  allocator_type alloc;

  //  template < typename Allocator
  void f() {
    traits::allocate(alloc, 1);
  }
};
*/

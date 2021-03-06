#include "main.h"

bool quiet_mode;

//
// global variable definition lambda
//

auto distance = [](auto first, auto last) { return last - first; };

auto equal = [](auto const first1, auto const last1, auto const first2,
                auto const last2) {
  // std::distance()
  if (distance(first1, last1) != distance(first2, last2))
    return false;

  // clang++-10.0.0-4ubuntu1
  // error: 'auto' deduced as 'array_iterator<array<int, 5> >'
  // in declaration of 'i' and deduced as 'int *' in declaration of 'j'
  // for (auto i = first1, auto j = first2; i != last1; ++i, ++j)
  auto i = first1;
  auto j = first2;
  for (; i != last1; ++i, ++j)
    if (*i != *j)
      return false;

  return true;
};

auto f = []() { return 1; };

//
// function definition
//

static void name_scope() {
  expect(__LINE__, 1, f());
  auto f = []() { return 2; };
  expect(__LINE__, 2, f());
  {
    expect(__LINE__, 2, f());
    auto f = []() { return 3; };
    expect(__LINE__, 3, f());
  }

  expect(__LINE__, 2, f());
}

static void lambda_expr() {
  expect(__LINE__, 43, [](auto x) { return x + 1; }(42));
}

#if ARRAY_ITERATOR == 1
// Support method
// 	std::for_each()
// Don't Support method
// 	std::fill()
// Note:
// 	bioperators don't check both iter refer same array.
template <typename Array>
struct array_iterator {
  using reference = typename Array::reference;

  Array &a;
  std::size_t i;

  array_iterator(Array &a, std::size_t i) : a(a), i(i) {}

  reference operator*() const { return a[i]; }

  // todo: std::size_t -> difference_type(can handle minus value)
  reference operator[](std::size_t n) const { return *(*this + n); }

  //
  // assignment operators
  //

  array_iterator &operator=(array_iterator iter) {
    i = iter.i;
    return *this;
  }

  array_iterator &operator+=(std::size_t n) {
    i += n;
    return *this;
  }

  //
  // increment/decrement operators
  //

  // pre
  array_iterator &operator++() {
    ++i;
    return *this;
  }

  // post
  array_iterator operator++(int) {
    array_iterator copy = *this;
    ++*this;
    return copy;
  }

  // pre
  array_iterator &operator--() {
    --i;
    return *this;
  }

  // post
  array_iterator operator--(int) {
    array_iterator copy = *this;
    --*this;
    return copy;
  }

  //
  // arithmetic operators
  //

  // result may be under 0. std::size_t, type of member i, is
  // unsigned integer.
  long long operator-(array_iterator const &iter) const {
    return i - iter.i;
  }

  array_iterator operator+(std::size_t n) const {
    array_iterator copy = *this;
    copy.i += n;
    return copy;
  }

  array_iterator operator-(std::size_t n) const { return *this + (-n); }

  //
  // comparison operators
  //

  bool operator==(array_iterator const &right) const {
    return i == right.i;
  }

  bool operator!=(array_iterator const &right) const {
    return !(*this == right);
  }

  bool operator<(array_iterator const &right) const {
    return i < right.i;
  }

  bool operator<=(array_iterator const &right) const {
    return i <= right.i;
  }

  bool operator>(array_iterator const &right) const {
    return i > right.i;
  }

  bool operator>=(array_iterator const &right) const {
    return i >= right.i;
  }
};

template <typename Array>
struct array_const_iterator {
  using const_reference = typename Array::const_reference;

  Array const &a;
  std::size_t i;

  array_const_iterator(Array const &a, std::size_t i) : a(a), i(i) {}

  // clang++-10.0.0-4ubuntu1
  // error: ISO C++ specifies that qualified reference to 'array_iterator'
  // is a constructor name rather than a type in this context, despite
  // preceding 'typename' keyword [-Werror,-Winjected-class-name]
  // array_const_iterator(
  //     typename array_iterator<Array>::array_iterator const &iter)
  array_const_iterator(typename Array::iterator const &iter)
      : a(iter.a), i(iter.i) {}

  const_reference operator*() const { return a[i]; }
  const_reference operator[](std::size_t n) const { return *(*this + n); }

  array_const_iterator &operator++() {
    ++i;
    return *this;
  }

  //
  // arithmetic operators
  //

  long long operator-(array_const_iterator const &iter) const {
    return i - iter.i;
  }

  //
  // comparison operators
  //

  bool operator==(array_const_iterator const &right) const {
    return i == right.i;
  }

  bool operator!=(array_const_iterator const &right) const {
    return !(*this == right);
  }
};
#endif

#if ARRAY_ITERATOR == 2
template <typename Array>
struct array_iterator {
  using pointer = typename Array::pointer;
  using reference = typename Array::reference;

  pointer p;

  //  array_iterator(pointer p) : p(p) {}

  array_iterator(Array &a, std::size_t i) { p = &a[i]; }

  reference operator*() { return *p; }

  reference operator[](std::size_t n) { return p[n]; }

  //
  // assignment operators
  //

  array_iterator &operator=(array_iterator iter) {
    p = iter.p;
    return *this;
  }

  array_iterator &operator+=(std::size_t n) {
    p += n;
    return *this;
  }

  //
  // inc/dec operators
  //

  // pre
  array_iterator &operator++() {
    ++p;
    return *this;
  }

  // post
  array_iterator operator++(int) {
    array_iterator copy = *this;
    ++*this;
    return copy;
  }

  // pre
  array_iterator &operator--() {
    --p;
    return *this;
  }

  // post
  array_iterator operator--(int) {
    array_iterator copy = *this;
    --*this;
    return copy;
  }

  //
  // arithmetic operators
  //

  // result may be under 0.
  long long operator-(array_iterator const &iter) const {
    return p - iter.p;
  }

  array_iterator operator+(std::size_t n) const {
    array_iterator copy = *this;
    copy += n;
    return copy;
  }

  array_iterator operator-(std::size_t n) const { return *this + (-n); }

  //
  // comparison operators
  //

  bool operator==(array_iterator const &right) const {
    return p == right.p;
  }

  bool operator!=(array_iterator const &right) const {
    return !(*this == right);
  }

  bool operator<(array_iterator const &right) const {
    return p < right.p;
  }
  bool operator<=(array_iterator const &right) const {
    return p <= right.p;
  }
  bool operator>(array_iterator const &right) const {
    return p > right.p;
  }
  bool operator>=(array_iterator const &right) const {
    return p >= right.p;
  }
};

template <typename Array>
struct array_const_iterator {
  using const_pointer = typename Array::const_pointer;
  using const_reference = typename Array::const_reference;

  const_pointer p;

  array_const_iterator(Array const &a, std::size_t i) { p = &a[i]; }

  array_const_iterator(typename Array::iterator const &iter) {
    p = iter.p;
  }

  const_reference operator*() const { return *p; }

  const_reference operator[](std::size_t n) const { return *(*this + n); }

  array_const_iterator &operator++() {
    ++p;
    return *this;
  }

  //
  // arithmetic operators
  //

  long long operator-(array_const_iterator const &iter) const {
    return p - iter.p;
  }

  //
  // comparison operators
  //

  bool operator==(array_const_iterator const &right) const {
    return p == right.p;
  }

  bool operator!=(array_const_iterator const &right) const {
    return !(*this == right);
  }
};
#endif

template <typename T, std::size_t N>
struct array {
#if ARRAY_ITERATOR < 3
  using iterator = array_iterator<array>;
  using const_iterator = array_const_iterator<array>;
#else
  using iterator = T *;
  using const_iterator = const T *;
#endif
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using const_pointer = T const *;
  using const_reference = T const &;

  using size_type = std::size_t;

  value_type storage[N];

  // clang-format off
#if ARRAY_ITERATOR < 3
  iterator       begin()        { return       iterator(*this, 0); }
  iterator       end()          { return       iterator(*this, N); }
  const_iterator begin()  const { return const_iterator(*this, 0); }
  const_iterator end()    const { return const_iterator(*this, N); }
  const_iterator cbegin() const { return const_iterator(*this, 0); }
  const_iterator cend()   const { return const_iterator(*this, N); }
#else
  iterator       begin()        { return &storage[0];  }
  iterator       end()          { return begin()  + N; }
  const_iterator begin()  const { return &storage[0];  }
  const_iterator end()    const { return begin()  + N; }
  const_iterator cbegin() const { return &storage[0];  }
  const_iterator cend()   const { return cbegin() + N; }
#endif
  // clang-format on

  reference operator[](size_type i) { return storage[i]; }
  const_reference operator[](size_type i) const { return storage[i]; }

  reference at(size_type i) {
    if (i < 0 || size() <= i)
      throw std::out_of_range("Error: Out of Range");
    return storage[i];
  }

  reference front() { return storage[0]; }
  const_reference front() const { return storage[0]; }
  reference back() { return storage[N - 1]; }
  const_reference back() const { return storage[N - 1]; }

  void fill(const_reference u) {
    // For clang-1200.0.32.27-x86_64-apple-darwin19.6.0, belows does not
    // work std::fill(std::begin(*this), std::end(*this), u);
    std::for_each(std::begin(*this), std::end(*this),
                  [=](auto &x) { x = u; });
  }

  size_type size() const { return N; }
};

struct cout_iterator {
  // --- boilerplate code
  // for output iterator doesn't use these.
  using difference_type = void;
  using value_type = void;
  using reference = void;
  using pointer = void;
  using iterator_category = std::output_iterator_tag;

  cout_iterator &operator*() { return *this; }
  cout_iterator &operator++() { return *this; }
  cout_iterator &operator++(int) { return *this; }
  // --- boilerplate code

  template <typename T>
  cout_iterator &operator=(T const &x) {
    std::cout << x;
    return *this;
  }
};

template <typename Container>
struct back_inserter {
  // --- boilerplate code
  // for output iterator doesn't use these.
  using difference_type = void;
  using value_type = void;
  using reference = void;
  using pointer = void;
  using iterator_category = std::output_iterator_tag;

  back_inserter &operator*() { return *this; }
  back_inserter &operator++() { return *this; }
  back_inserter &operator++(int) { return *this; }
  // --- boilerplate code

  Container &c;
  back_inserter(Container &c) : c(c) {}
  back_inserter &operator=(const typename Container::value_type &value) {
    c.push_back(value);
    return *this;
  }
};

namespace old {
template <typename Container>
struct back_insert_iterator {
  // --- boilerplate code
  // for output iterator doesn't use these.
  using difference_type = void;
  using value_type = void;
  using reference = void;
  using pointer = void;
  using iterator_category = std::output_iterator_tag;

  back_insert_iterator &operator*() { return *this; }
  back_insert_iterator &operator++() { return *this; }
  back_insert_iterator &operator++(int) { return *this; }
  // --- boilerplate code

  back_insert_iterator(Container &c) : c(&c) {}
  back_insert_iterator &
  operator=(const typename Container::value_type &value) {
    c->push_back(value);
    return *this;
  }

  Container *c;
};

template <typename Container>
back_insert_iterator<Container> back_inserter(Container &c) {
  return back_insert_iterator<Container>(c);
}
} // namespace old

template <typename T>
struct cin_iterator {
  // --- boilerplate code
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = T &;
  using pointer = T *;
  using iterator_category = std::input_iterator_tag;
  // --- boilerplate code

  cin_iterator(bool fail = false) : fail(fail) { ++*this; }

  // const reference &operator*() const { return value; }
  reference &operator*() { return value; }

  cin_iterator &operator++() {
    if (!fail) {
      std::cin >> value;
      fail = std::cin.fail();
    }
    return *this;
  }

  cin_iterator operator++(int) {
    auto old = *this;
    ++*this;
    return old;
  }

  bool fail;
  value_type value;
};

template <typename T>
bool operator==(cin_iterator<T> const &l, cin_iterator<T> const &r) {
  return l.fail == r.fail;
}

template <typename T>
bool operator!=(cin_iterator<T> const &l, cin_iterator<T> const &r) {
  return !(l == r);
}

template <typename InputIterator>
void print(InputIterator iter, InputIterator end_iter) {
  while (iter != end_iter) {
    std::cout << *iter;
    ++iter;
  }
}

template <typename T>
struct iota_iterator {
  // --- boilerplate code
  using difference_type = long long;
  using value_type = T;
  using reference = T &;
  using pointer = T *;
  using iterator_category = std::random_access_iterator_tag;
  // --- boilerplate code

  T value;
  iota_iterator(T value = 0) : value(value) {}

  reference operator*() noexcept { return value; }
  // const reference operator*() const noexcept { return value; }
  reference operator*() const noexcept { return value; }
  value_type operator[](difference_type n) const { return value + n; }

  //
  // inc/dec
  //

  iota_iterator &operator++() noexcept {
    ++value;
    return *this;
  }

  iota_iterator operator++(int) noexcept {
    auto temp = *this;
    ++*this;
    return temp;
  }

  iota_iterator &operator--() noexcept {
    --value;
    return *this;
  }

  iota_iterator operator--(int) noexcept {
    auto temp = *this;
    --*this;
    return temp;
  }

  //
  // assignment operators
  //

  iota_iterator &operator+=(difference_type n) { // original
    value += n;
    return *this;
  }
  iota_iterator &operator-=(difference_type n) { // original
    value -= n;
    return *this;
  }

  //
  // arithmetic operators
  //

  iota_iterator operator+(difference_type n) const {
    auto temp = *this;
    temp += n;
    return temp;
  }

  iota_iterator operator-(difference_type n) const {
    auto temp = *this;
    temp -= n;
    return temp;
  }

  difference_type operator-(iota_iterator const &r) const {
    return value - r.value;
  }

  //
  // comparision operators
  //

  bool operator==(iota_iterator<T> const &r) const noexcept {
    return value == r.value;
  }
  bool operator!=(iota_iterator<T> const &r) const noexcept {
    return !(*this == r);
  }
  bool operator<(iota_iterator<T> const &r) const noexcept {
    return value < r.value;
  }
  bool operator<=(iota_iterator<T> const &r) const noexcept {
    return value <= r.value;
  }
  bool operator>(iota_iterator<T> const &r) const noexcept {
    return value > r.value;
  }
  bool operator>=(iota_iterator<T> const &r) const noexcept {
    return value >= r.value;
  }
};

// clang-format off
template <typename T> iota_iterator<T>
operator+(typename iota_iterator<T>::difference_type n,
	  iota_iterator<T> const &i) {
  return i + n;
}

template <typename T> iota_iterator<T>
operator-(typename iota_iterator<T>::difference_type n,
          iota_iterator<T> const &i) {
  iota_iterator<T> temp(n);
  
  return temp - i;
}
// clang-format on

template <typename T>
struct forward_link_list {
  T value;
  forward_link_list *next;
};

template <typename T>
struct forward_link_list_iterator {
  // --- boilerplate code
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = T &;
  using pointer = T *;
  using iterator_category = std::forward_iterator_tag;
  // --- boilerplate code

  forward_link_list<T> *ptr;

  forward_link_list_iterator(forward_link_list<T> *ptr) : ptr(ptr) {}

  T &operator*() noexcept { return ptr->value; }

  forward_link_list_iterator &operator++() noexcept {
    ptr = ptr->next;
    return *this;
  }

  forward_link_list_iterator operator++(int) noexcept {
    auto tmp = *this;
    ptr = ptr->next;
    return tmp;
  }

  bool operator==(forward_link_list_iterator const &r) const {
    return ptr == r.ptr;
  }

  bool operator!=(forward_link_list_iterator const &r) const {
    return !(*this == r);
  }
};

template <typename T>
forward_link_list<T> &next(forward_link_list<T> &list) noexcept {
  return *list.next;
}

template <typename T>
struct bidirectional_link_list {
  T value;

  bidirectional_link_list *prev;
  bidirectional_link_list *next;

  bidirectional_link_list(T value, bidirectional_link_list *prev,
                          bidirectional_link_list *next) {
    this->value = value;
    this->prev = prev;
    this->next = next;
  }

  /*
  void push_front(T value) {
    bidirectional_link_list temp(value, nullptr, this);
    this->prev = &temp;

    this = &temp;
  }
  */
};

template <typename T>
struct bidirectional_link_list_iterator {
  using iterator = bidirectional_link_list_iterator;

  // --- boilerplate code
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = T &;
  using pointer = T *;
  using iterator_category = std::bidirectional_iterator_tag;
  // --- boilerplate code

  bidirectional_link_list<T> *ptr;

  bidirectional_link_list_iterator(bidirectional_link_list<T> *ptr)
      : ptr(ptr) {}

  T &operator*() noexcept { return ptr->value; }

  iterator &operator++() noexcept {
    ptr = ptr->next;
    return *this;
  }
  iterator operator++(int) noexcept {
    auto temp = *this;
    ++*this;
    return temp;
  }

  iterator &operator--() noexcept {
    ptr = ptr->prev;
    return *this;
  }
  iterator operator--(int) noexcept {
    auto temp = *this;
    --*this;
    return temp;
  }

  bool operator==(iterator const &r) const noexcept {
    return ptr == r.ptr;
  }
};

namespace ns {
// ns::forward_link_list_iterator
template <typename List>
struct forward_link_list_iterator {
  // --- boilerplate code
  using difference_type = std::ptrdiff_t;
  using value_type = typename List::value_type;
  using reference = typename List::reference;

  //  using pointer = T *;
  using iterator_category = std::forward_iterator_tag;
  // --- boilerplate code

  List *ptr;

  forward_link_list_iterator(List *ptr) : ptr(ptr) {}

  reference operator*() noexcept { return ptr->value; }

  forward_link_list_iterator &operator++() noexcept {
    ptr = ptr->next;
    return *this;
  }

  forward_link_list_iterator operator++(int) noexcept {
    auto temp = *this;
    ptr = ptr->next;
    return temp;
  }

  bool operator==(forward_link_list_iterator const &r) const {
    return ptr == r.ptr;
  }

  bool operator!=(forward_link_list_iterator const &r) const {
    return !(*this == r);
  }
};

// ns::forward_link_list
template <typename T>
struct forward_link_list {
  // using iterator = forward_link_list_iterator<forward_link_list<T>>;
  using iterator = forward_link_list_iterator<forward_link_list>;

  // error: use of class template 'forward_link_list_iterator' requires
  // template arguments
  //	using iterator = forward_link_list_iterator;
  using value_type = T;
  using reference = T &;

  T value;
  forward_link_list *next;

  // clang-format off
  iterator begin() { return iterator(this); }
  iterator end()   { return iterator(nullptr); }
  // clang-format on  
};

  
} // namespace ns

static void test_array() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  const array<int, 5> ca = {1, 2, 3, 4, 5};

  auto &refa = a[1]; // auto = int
  refa += 2;
  expect(__LINE__, 4, a[1]);
  refa = a[2]; // equivalent to a[1] = a[2]
               // refa is reference to a[1]
  expect(__LINE__, 3, a[1]);

  auto &refca = ca[1]; // auto = int const
  expect(__LINE__, 2, refca);
  // error: increment of read-only reference ‘refca’
  // ++refca;
  // error: assignment of read-only reference ‘refca’
  // refca = ca[2];

  auto const &crefa = a[2]; // auto = int
  expect(__LINE__, 3, crefa);
  // error:increment of read-only reference ‘crefa’
  // ++crefa;
  // error: assignment of read-only reference ‘crefa
  // crefa = a[3];

  auto const &crefca = ca[2]; // auto = int
  expect(__LINE__, 3, crefca);
  // error: increment of read-only reference ‘crefca’
  // ++crefca;
  //  crefca = ca[3];

  int &irefa = a[3];
  expect(__LINE__, 5, ++irefa);
  // error: binding reference of type ‘int&’ to ‘const int’ discards
  // qualifiers
  // int &irefca = ca[3];
  int const &icrefa = a[4];
  expect(__LINE__, 5, icrefa);
  int const &icrefca = ca[3];
  expect(__LINE__, 4, icrefca);

  expect(__LINE__, 5ul, a.size());
  expect(__LINE__, 5ul, ca.size());

  expect(__LINE__, 1, a.front()++);
  expect(__LINE__, 2, a[0]);

  expect(__LINE__, 1, ca.front());

  expect(__LINE__, 5, a.back()++);
  expect(__LINE__, 6, a[4]);

  expect(__LINE__, 5, ca.back());

  std::array<int, 5> a0 = {2, 3, 3, 5, 6};

  // std::equal()
  expect(__LINE__, true,
         equal(std::begin(a), std::end(a), std::begin(a0), std::end(a0)));

  a.fill(0);
  std::array<int, 5> a1 = {0, 0, 0, 0, 0};
  expect(__LINE__, true,
         equal(std::begin(a), std::end(a), std::begin(a1), std::end(a1)));
}

static void test_array_iterator() {
  array<int, 5> a = {1, 2, 3, 4, 5};

  auto iter = a.begin();
  expect(__LINE__, 1, *iter);
  expect(__LINE__, 2, *++iter); // pre-inc
  expect(__LINE__, 2, *iter++); // post-inc
  expect(__LINE__, 3, *iter);   // post-inc side-effect

  auto iter2 = a.end();
  expect(__LINE__, 5, *--iter2); // pre-dec
  expect(__LINE__, 5, *iter2--); // post-dec
  expect(__LINE__, 4, *iter2);   // post-dec side-effect

  // operator==, operator!=
  expect(__LINE__, false, iter == iter2);
  expect(__LINE__, true, iter != iter2);
  --iter2;
  expect(__LINE__, true, iter == iter2);
}

static void test_for_each() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  std::for_each(std::begin(a), std::end(a), [](auto &x) { ++x; });

  std::array<int, 5> a0 = {2, 3, 4, 5, 6};
  expect(__LINE__, true,
         equal(std::begin(a), std::end(a), std::begin(a0), std::end(a0)));
}

template <typename Array>
void copy(Array &dst, Array const &src) {
  for (std::size_t i = 0; i != src.size(); ++i)
    dst[i] = src[i];
}

static void test_copy() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  array<int, 5> a_;
  copy(a_, a);
  expect(__LINE__, true,
         equal(std::begin(a), std::end(a), std::begin(a_), std::end(a_)));
}

struct S {
  int data{};

  S(int d) : data(d) {}
  void f() {}
  int g() { return data + 100; }
  // error: assignment of member ‘S::data’ in read-only object
  // void g() const { data = 42;};
  int g() const { return data + 200; }

  int &get() { return data; }
  // error: binding reference of type ‘int&’ to ‘const int’
  // discards qualifiers
  //  int & get() const { return data; }
  int const &get() const { return data; }
};

static void test_const() {
  int const cx = 1;
  expect(__LINE__, 1, cx);
  // error: assignment of read-only variable ‘cx’
  //  cx = 0:

  S s(1);
  S const cs(2);
  // error: passing ‘const S’ as ‘this’ argument discards qualifiers
  // [-fpermissive]
  // cs.f();
  expect(__LINE__, 101, s.g());  //  s.g() call S::g()
  expect(__LINE__, 202, cs.g()); // cs.g() call S::g() const

  s.get() = 3;
  expect(__LINE__, 3, s.data);
  expect(__LINE__, 2, cs.get()); // ? get() returns constant ref of int
  // error: assignment of read-only location ‘cs.S::get()’
  // cs.get() = 4;
}

static void test_array_iterator_index() {
  array<int, 5> a = {1, 2, 3, 4, 5};

  auto iter = std::begin(a);
  iter += 2;
  expect(__LINE__, 3, *iter);
  iter = iter + 2;
  expect(__LINE__, 5, *iter);

  expect(__LINE__, 4, *(iter - 1));

  expect(__LINE__, 3, iter[-2]);
}

static void test_array_iterator_comparison() {
  array<int, 5> ar = {1, 2, 3, 4, 5};

  auto a = std::begin(ar);
  auto b = a + 1;

  expect(__LINE__, true, a < b);
  expect(__LINE__, true, a <= b);
  expect(__LINE__, false, a > b);
  expect(__LINE__, false, a >= b);
}

static void const_iterator1() {
  using Array = std::array<int, 5>;
  Array a = {1, 2, 3, 4, 5};

  Array::iterator i = std::begin(a);
  const Array::iterator c_i = std::begin(a);
  Array::const_iterator ci = std::cbegin(a);
  const Array::const_iterator c_ci = std::cbegin(a);

  // increment of read-only variable
  // ++c_i;
  // ++c_ci;
  auto next_itr1 = c_i + 1;  // Okay not change c_i
  auto next_itr2 = c_ci + 1; // Okay not change c_ci

  expect(__LINE__, true, i == c_i);
  expect(__LINE__, true, ci == c_ci);
  expect(__LINE__, true, next_itr1 == next_itr2);
}

static void const_iterator2() {
  using Array = std::array<int, 5>;
  Array a = {1, 2, 3, 4, 5};
  const Array ca = {1, 2, 3, 4, 5};

  // const_iterator: ci_a
  auto ci_a = std::cbegin(a);

  // const iterator: ci_ca
  auto ci_ca = std::begin(ca);

  // const_iterator: ci_a2
  auto i_a = std::begin(a);
  Array::const_iterator ci_a2 = i_a;

  // const_iterator can change itself
  ++ci_a2;
  // error: assignment of read-only location ‘* ci_a2’
  // *ci_a2 = 2;

  expect(__LINE__, false, ci_a == ci_a2);
  expect(__LINE__, false, ci_a == ci_ca);
}

static void const_iterator3() {
  using Array = array<int, 5>;
  Array a = {1, 2, 3, 4, 5};

  auto total{0};
  for (Array::const_iterator i = std::begin(a); i != std::end(a); ++i)
    total += *i;

  expect(__LINE__, 15, total);
}

// begin()/end() const
static void const_iterator4() {
  using Array = array<int, 5>;
  Array a = {1, 2, 3, 4, 5};

  auto total{0};

  // std::cbegin(a) calls a.begin() const.
  for (Array::const_iterator i = std::cbegin(a); i != std::cend(a); ++i)
    total += *i;

  expect(__LINE__, 15, total);
}

// cbegin()/cend()
static void const_iterator5() {
  using Array = array<int, 5>;
  const Array a = {1, 2, 3, 4, 5};

  auto total{0};
  for (Array::const_iterator i = a.cbegin(); i != a.cend(); ++i)
    total += *i;

  expect(__LINE__, 15, total);
}

static void test_at() {
  array<int, 1> a = {1};
  try {
    expect(__LINE__, 1, a.at(1000));
  } catch (std::out_of_range &e) {
    expect(__LINE__, "Error: Out of Range"s, e.what());
  }
}

static void test_type() {
  // char const*
  auto str1 = "without s";

  // std::__cxx11::basic_string<...>
  auto str2 = "with s"s;

  expect(__LINE__, false, str2 == str1);
}

template <typename RandomAccessIterator>
void test_iterator_0(RandomAccessIterator i, std::size_t n) {
  [[maybe_unused]] RandomAccessIterator iter;
  iter = i + n;
  iter = i - n;
  iter = n + i;

  #ifdef ISSUE_1
  iter = n - i;
  #endif

  iter = i + (-n);

  iter = i += n;
  iter = i -= n;

  // same as *(i + n)
  static_cast<void>(i[n]);
}

template <typename RandomAccessIterator>
void test_iterator_1(RandomAccessIterator a, RandomAccessIterator b) {
  //  [[maybe_unused]]RandomAccessIterator iter;
  [[maybe_unused]] long long diff;
  diff = b - a;
  diff = a - b;
}

template <typename RandomAccessIterator>
void test_iterator_3(RandomAccessIterator a, RandomAccessIterator b) {
  // clang-format off
  static_cast<void>( a <  b );
  static_cast<void>( a <= b );
  static_cast<void>( a >= b );
  static_cast<void>( a >  b );
  // clang-format on  
}

template <typename BidirectionalIterator>
void test_iterator_5(BidirectionalIterator iter) {
  BidirectionalIterator temp = iter;
  static_cast<void>( --temp );
  temp = iter;
  static_cast<void>( temp-- );  
}

template <typename ForwardIterator>
void test_multipath_guarantee(ForwardIterator i) {
  auto prev = i;
  ++i;
  ++prev;
  expect(__LINE__, true, i == prev);
  expect(__LINE__, true, *i == *prev);
}

template <typename InputIterator>
void test_iterator_7(InputIterator a, InputIterator b) {
  // clang-format off
  static_cast<void>( a == b );
  static_cast<void>( a != b );
  
  static_cast<void>( *a );
  // a->m;

  static_cast<void>( ++a );
  static_cast<void>( a++ );
  // clang-format on
}

template <typename OutputIterator, typename V>
void test_iterator_8(OutputIterator iter, V v) {
  // clang-format off
  static_cast<void>( *iter = v );

  static_cast<void>( ++iter );
  static_cast<void>( iter++ );
  // clang-format on
}

template <typename tag, typename Iterator>
constexpr bool is_category_of() {
  using iter_tag =
      typename std::iterator_traits<Iterator>::iterator_category;
  return std::is_base_of_v<tag, iter_tag>;
}

/**
  Random access iterator
  - std::vector<T>
  - std::array<T,N>
*/
static void test_random_access_iter() {
  using iterator = std::vector<int>::iterator;
  expect(__LINE__, true,
         is_category_of<std::random_access_iterator_tag, iterator>());

  std::vector<int> a = {0, 1, 2, 3, 4};
  auto iter = std::begin(a);
  auto end_iter = std::end(a);

  test_iterator_0(iter, 5);
  test_iterator_1(iter, end_iter);
  test_iterator_1(end_iter, iter);
  test_iterator_3(iter, end_iter);
  test_iterator_5(iter);
  test_multipath_guarantee(iter);
  test_iterator_7(iter, end_iter);
  test_iterator_8(iter, 8);
}

/**
 * std::array<T, N>
 */
static void test_random_access_iter2() {
  using iterator = std::array<int, 5>::iterator;
  expect(__LINE__, true,
         is_category_of<std::random_access_iterator_tag, iterator>());

  std::array<int, 5> a = {0, 1, 2, 3, 4};

  auto iter = std::begin(a);
  auto end_iter = std::end(a);

  test_iterator_0(iter, 5);
  test_iterator_1(iter, end_iter);
  test_iterator_1(end_iter, iter);
  test_iterator_3(iter, end_iter);
  test_iterator_5(iter);
  test_multipath_guarantee(iter);
  test_iterator_7(iter, end_iter);
  test_iterator_8(iter, 8);
}

/**
 * array
 * TODO: passing by implementation pattern 1 and 2.
 */
static void test_random_access_iter3() {
  using iterator = array<int, 5>::iterator;
  expect(__LINE__, true,
         is_category_of<std::random_access_iterator_tag, iterator>());

  array<int, 5> a = {0, 1, 2, 3, 4};

  auto iter = std::begin(a);
  auto end_iter = std::end(a);

  test_iterator_0(iter, 5);
  test_iterator_1(iter, end_iter);
  test_iterator_1(end_iter, iter);
  test_iterator_3(iter, end_iter);
  test_iterator_5(iter);
  test_multipath_guarantee(iter);
  test_iterator_7(iter, end_iter);
  test_iterator_8(iter, 8);
}

/**
 * iota_iterator
 */
static void test_random_access_iter4() {
  using iterator = iota_iterator<int>;
  expect(__LINE__, true,
         is_category_of<std::random_access_iterator_tag, iterator>());

  iota_iterator<int> iter, end_iter(10);

  // clang-format off
  expect(__LINE__,  0, *iter++);  
  expect(__LINE__,  2, *++iter);

  expect(__LINE__,  2, *iter--);
  expect(__LINE__,  0, *--iter);

  expect(__LINE__,  0, iter[0]);
  expect(__LINE__,  1, iter[1]);

  expect(__LINE__,  2, *(iter+=2));
  expect(__LINE__,  0, *(iter-=2));

  iter+=1;
  expect(__LINE__,  4, *(iter + 3));
  expect(__LINE__, -2, *(iter - 3));
  expect(__LINE__,  9, end_iter - iter);

  // iterator<T>::difference_type + iterator<T>
  expect(__LINE__,  4, *(3 + iter));
  // iterator<T>::difference_type - iterator<T>  
  expect(__LINE__,  2, *(3 - iter));
      
  expect(__LINE__, false, iter == end_iter);
  expect(__LINE__, true,  iter+9 == end_iter);
  expect(__LINE__, true,  iter != end_iter);
  
  expect(__LINE__, true,  iter   <  iter+1 );
  expect(__LINE__, false, iter   <  iter   );
  expect(__LINE__, true,  iter   <= iter   );
  expect(__LINE__, false, iter   <= iter-1 );
  expect(__LINE__, true,  iter+1 >  iter   );
  expect(__LINE__, false, iter   >  iter   );
  expect(__LINE__, true,  iter   >= iter   );	 
  expect(__LINE__, false, iter-1 >= iter   );	 
  
  test_iterator_0(iter, 5);
  test_iterator_1(iter, end_iter);
  test_iterator_1(end_iter, iter);
  test_iterator_3(iter, end_iter);
  test_iterator_5(iter);
  test_multipath_guarantee(iter);
  test_iterator_7(iter, end_iter);
  test_iterator_8(iter, 8);
  // clang-format on
}

/**
  Bidirectional iterator
  - std::list<T>
*/
static void test_bidirectional_iter() {
  using iterator = std::list<int>::iterator;
  expect(__LINE__, false,
         is_category_of<std::random_access_iterator_tag, iterator>());
  expect(__LINE__, true,
         is_category_of<std::bidirectional_iterator_tag, iterator>());

  std::list<int> list;

  list.push_front(0);
  list.push_front(1);
  list.push_front(2);

  auto iter = std::begin(list);
  auto end_iter = std::end(list);

  test_iterator_5(iter);
  test_multipath_guarantee(iter);
  test_iterator_7(iter, end_iter);
  test_iterator_8(iter, 8);
}

/**
 * bidirectional_link_list_iterator
 */
static void test_bidirectional_iter3() {
  using iterator = bidirectional_link_list_iterator<int>;
  expect(__LINE__, true,
         is_category_of<std::bidirectional_iterator_tag, iterator>());

  bidirectional_link_list<int> list0(0, nullptr, nullptr);
  bidirectional_link_list<int> list1(1, &list0, nullptr);
  bidirectional_link_list<int> list2(2, &list1, nullptr);
  list0.next = &list1;
  list1.next = &list2;

  expect(__LINE__, nullptr, list0.prev);
  expect(__LINE__, 0, list0.value);
  expect(__LINE__, 1, list0.next->value);

  bidirectional_link_list_iterator iter(&list0), last(list2.next);
  std::vector<int> tmp(3);

  std::copy(iter, last, std::begin(tmp));
  expect(__LINE__, 0, tmp[0]);
  expect(__LINE__, 2, tmp[2]);

  test_iterator_5(iter);
  test_multipath_guarantee(iter);
  test_iterator_7(iter, last);
  test_iterator_8(iter, 8);
}

/**
 forward iterator
 - std::forward_lsit<T>
*/
static void test_forward_iter() {
  using iterator = std::forward_list<int>::iterator;
  expect(__LINE__, false,
         is_category_of<std::bidirectional_iterator_tag, iterator>());
  expect(__LINE__, true,
         is_category_of<std::forward_iterator_tag, iterator>());
  std::forward_list<int> list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);

  auto iter = std::begin(list);
  auto end_iter = std::end(list);

  test_multipath_guarantee(iter);
  test_iterator_7(iter, end_iter);
  test_iterator_8(iter, 8);
}

/**
 * forward_link_list_iterator
 */
static void test_forward_iter3() {
  using iterator = forward_link_list_iterator<int>;
  expect(__LINE__, true,
         is_category_of<std::forward_iterator_tag, iterator>());

  forward_link_list<int> list2 = {2, nullptr};
  forward_link_list<int> list1 = {1, &list2};
  forward_link_list<int> list0 = {0, &list1};

  forward_link_list_iterator<int> iter(&list0), last(list2.next);
  std::vector<int> v(3);
  std::copy(iter, last, std::begin(v));
  expect(__LINE__, 0, v[0]);
  expect(__LINE__, 2, v[2]);

  test_multipath_guarantee(iter);
  test_iterator_7(iter, last);
  test_iterator_8(iter, 8);
}

/**
 * ns::forward_link_list_iterator
 */
static void test_forward_iter4() {
  //  using namespace ns;

  // using iterator =
  //    ns::forward_link_list_iterator<ns::forward_link_list<int>>;
  using iterator = ns::forward_link_list<int>::iterator;

// TODO:
#ifdef ISSUE_2
  expect(__LINE__, true,
         is_category_of<std::forward_iterator_tag, iterator>());
#endif

  ns::forward_link_list<int> list2 = {2, nullptr};
  ns::forward_link_list<int> list1 = {1, &list2};
  ns::forward_link_list<int> list0 = {0, &list1};

  //  ns::forward_link_list_iterator<int> iter
  std::vector<int> v(3);
  //  std::copy(std::begin(list0), std::end(list0), std::begin(v));
  auto iter_v = std::begin(v);
  for (iterator iter = std::begin(list0); iter != std::end(list0);)
    *iter_v++ = *iter++;
  expect(__LINE__, 0, v[0]);
  expect(__LINE__, 2, v[2]);

  iterator iter = std::begin(list0);
  iterator last = std::end(list0);

  test_multipath_guarantee(iter);
  test_iterator_7(iter, last);
  test_iterator_8(iter, 8);
}

/**
 * input iterator
 * - std::istream_iterator<T>
 */
static void test_input_iter() {
  using iterator = std::istream_iterator<int>;
  expect(__LINE__, false,
         is_category_of<std::forward_iterator_tag, iterator>());
  expect(__LINE__, true,
         is_category_of<std::input_iterator_tag, iterator>());

  std::istream_iterator<int> iter(std::cin);
  test_iterator_7(iter, iter);
}

/**
 * cin_iterator
 */
static void test_input_iter2() {
  using iterator = cin_iterator<int>;
  expect(__LINE__, true,
         is_category_of<std::input_iterator_tag, iterator>());

  cin_iterator<int> iter;
  test_iterator_7(iter, iter);
}

/**
 * output iterator
 *  - std::ostream_iterator<T>
 *  - std::back_insert_iterator<Container> std::back_inserter(Container
 * &x)
 */
static void test_output_iter() {
  //
  // std::ostream_iterator<T>
  //
  using iterator = std::ostream_iterator<int>;
  expect(__LINE__, false,
         is_category_of<std::forward_iterator_tag, iterator>());
  expect(__LINE__, true,
         is_category_of<std::output_iterator_tag, iterator>());

  iterator iter(std::cout);
  test_iterator_8(iter, 8);
}

/**
 * cout_iterator
 */
static void test_output_iter2() {
  using iterator = cout_iterator;
  expect(__LINE__, true,
         is_category_of<std::output_iterator_tag, iterator>());

  cout_iterator iter2;
  test_iterator_8(iter2, 8);
}

/**
 * back_inserter
 */
static void test_output_iter3() {
  using iterator = back_inserter<std::vector<int>>;
  expect(__LINE__, true,
         is_category_of<std::output_iterator_tag, iterator>());

  std::vector<int> v;
  auto iter = back_inserter(v);

  test_iterator_8(iter, 8);
}

/**
 * old::back_inserter
 */
static void test_output_iter4() {
  using iterator = old::back_insert_iterator<std::vector<int>>;
  expect(__LINE__, true,
         is_category_of<std::output_iterator_tag, iterator>());

  std::vector<int> v;
  auto iter = old::back_inserter(v);

  test_iterator_8(iter, 8);
}

template <typename Iterator>
void test_iterator_toraits_0(Iterator i, Iterator j) {
  [[maybe_unused]]
  typename std::iterator_traits<Iterator>::difference_type diff = j - i;
  typename std::iterator_traits<Iterator>::value_type v = *i;
  [[maybe_unused]] typename std::iterator_traits<Iterator>::pointer p =
      &v;
  [[maybe_unused]] typename std::iterator_traits<Iterator>::reference r =
      v;
}

static void test_iterator_traits() {
  std::vector<int> v = {0, 1, 2, 3, 4};
  auto iter = std::begin(v);
  test_iterator_toraits_0(iter, iter);
}

static void test_output_iterator() {
  std::array<int, 5> a = {0, 1, 2, 3, 4};
  std::vector<int> v(5);
  std::copy(std::begin(a), std::end(a), std::begin(v));
  expect(__LINE__, 4, v.back());
}

static void test_output_iterator2() {
  std::array<int, 5> a = {0, 1, 2, 3, 4};
  std::vector<int> tmp;
  auto out = std::back_inserter(tmp);
  std::copy(std::begin(a), std::end(a), out);
  expect(__LINE__, 4, tmp[4]);
}

static void test_output_iterator3() {
  std::array<int, 5> a = {0, 1, 2, 3, 4};
  std::vector<int> tmp;
  auto out = old::back_inserter(tmp);
  std::copy(std::begin(a), std::end(a), out);
  expect(__LINE__, 4, tmp[4]);
}

/**
 * back_inserter
 */
static void test_back_inserter() {
  std::array<int, 5> a = {0, 1, 2, 3, 4};
  std::vector<int> tmp;
  auto out = back_inserter(tmp);
  std::copy(std::begin(a), std::end(a), out);
  expect(__LINE__, 4, tmp[4]);
}

static void test_cout_iterator() {
  std::vector<int> v = {0, 1, 2, 3, 4};
  cout_iterator out;

  std::copy(std::begin(v), std::end(v), out);
}

static void test_cin_iterator() {
  cin_iterator<int> input, fail(true);
  std::vector<int> buffer;

  [[maybe_unused]] bool b = input == fail;

  cout_iterator out;
  std::copy(input, fail, out);
}

static void test_print() {
  std::vector<int> v = {0, 1, 2, 3, 4};
  print(std::begin(v), std::end(v));

  if (false) {
    cin_iterator<int> iter, fail(true);
    print(iter, fail);
  }

  {
    std::istream_iterator<int> iter(std::cin), end_iter;
    std::vector<int> v;
    std::copy(iter, end_iter, std::back_inserter(v));
    print(std::begin(v), std::end(v));
  }
}

static void test_iota_iterator() {
  iota_iterator<int> iter;
  expect(__LINE__, 0, *iter);
  expect(__LINE__, 0, *iter++);
  expect(__LINE__, 2, *++iter);

  iota_iterator first(0), last(10);
  expect(__LINE__, false, first == last);

  return;
  std::vector<int> v;
  std::copy(first, last, std::back_inserter(v));
  expect(__LINE__, 10, v.size());
  expect(__LINE__, 0, v[0]);
  expect(__LINE__, 9, v[9]);
}

static void test_forward_link_list() {
  forward_link_list<int> list3{3, nullptr};
  forward_link_list<int> list2{2, &list3};
  forward_link_list<int> list1{1, &list2};
  forward_link_list<int> list0{0, &list1};

  expect(__LINE__, 0, list0.value);
  expect(__LINE__, 1, list0.next->value);
  expect(__LINE__, 2, list0.next->next->value);
  expect(__LINE__, 1, next(list0).value);
  expect(__LINE__, 2, next(next(list0)).value);
}

static void test_forward_link_list_iterator() {
  forward_link_list<int> list3{3, nullptr};
  forward_link_list<int> list2{2, &list3};
  forward_link_list<int> list1{1, &list2};
  forward_link_list<int> list0{0, &list1};

  forward_link_list_iterator<int> iter(&list0);
  expect(__LINE__, 0, *iter);
  expect(__LINE__, 1, *++iter);
}

namespace ns {
static void test_forward_link_list() {

  forward_link_list<int> list3{3, nullptr};
  forward_link_list<int> list2{2, &list3};
  forward_link_list<int> list1{1, &list2};
  forward_link_list<int> list0{0, &list1};

  expect(__LINE__, 0, list0.value);
  expect(__LINE__, 1, list0.next->value);
  expect(__LINE__, 2, list0.next->next->value);
}

static void test_forward_link_list_iterator() {
  forward_link_list<int> list3{3, nullptr};
  forward_link_list<int> list2{2, &list3};
  forward_link_list<int> list1{1, &list2};
  forward_link_list<int> list0{0, &list1};

  auto iter = list0.begin();
  expect(__LINE__, 0, *iter);
  expect(__LINE__, 1, *++iter);
}
} // namespace ns

// forward_iterator
// std::forward_list<T>
static void test_advance0_0() {
  std::forward_list<int> list = {0, 1, 2, 3, 4};

  auto begin = std::begin(list);
  auto last = std::end(list);

  expect(__LINE__, 1, *std::next(begin));
  expect(__LINE__, 3, *std::next(begin, 3));

  // forward_iterator_tag:
  // below codes does not work.
  //   	std::prev(++begin);
  //	std::advance(++begin, -1);

  auto iter = begin;
  expect(__LINE__, 0, *iter);
  std::advance(iter, 1);
  expect(__LINE__, 1, *iter);
  std::advance(iter, 3);
  expect(__LINE__, 4, *iter);

  std::advance(iter, 0);
  expect(__LINE__, 4, *iter);

  expect(__LINE__, 5, std::distance(begin, last));
}

// forward_iterator
// ns::forward_link_list<T>
static void test_advance0_1() {
  ns::forward_link_list<int> list4{4, nullptr};
  ns::forward_link_list<int> list3{3, &list4};
  ns::forward_link_list<int> list2{2, &list3};
  ns::forward_link_list<int> list1{1, &list2};
  ns::forward_link_list<int> list0{0, &list1};

  [[maybe_unused]] auto iter = std::begin(list0);
  [[maybe_unused]] auto last = std::end(list0);

  // TODO
  // auto iter2 = std::next(iter, 2);

#ifdef ISSUE_3
  // forward_link_list_iterator::difference_type
  std::advance(iter, 1);
  expect(__LINE__, 0, *iter);
  std::advance(iter, 1);
  expect(__LINE__, 1, *iter);
  std::advance(iter, 3);
  expect(__LINE__, 4, *iter);

  std::advance(iter, -2);
  expect(__LINE__, 2, *iter);
  std::advance(iter, 0);
  expect(__LINE__, 2, *iter);

  // forward_link_list_iterator::difference_type
  std::distance(iter, last);
#endif
}

// bidirectional_iterator
// std::list<T>
static void test_advance1_0() {
  std::list<int> list = {0, 1, 2, 3, 4};

  auto begin = std::begin(list);
  auto end = std::end(list);

  expect(__LINE__, 1, *std::next(begin));
  expect(__LINE__, 3, *std::next(begin, 3));
  // amazing.  the following code works.
  expect(__LINE__, 4, *std::prev(end));
  expect(__LINE__, 2, *std::prev(end, 3));

  auto iter = begin;
  expect(__LINE__, 0, *iter);
  std::advance(iter, 1);
  expect(__LINE__, 1, *iter);
  std::advance(iter, 3);
  expect(__LINE__, 4, *iter);

  std::advance(iter, -2);
  expect(__LINE__, 2, *iter);
  std::advance(iter, 0);
  expect(__LINE__, 2, *iter);

  expect(__LINE__, 3, std::distance(iter, end));
}

// random_access
// std::array
static void test_advance2_0() {
  std::array<int, 5> a = {0, 1, 2, 3, 4};

  auto begin = std::begin(a);
  auto end = std::end(a);

  expect(__LINE__, 1, *std::next(begin));
  expect(__LINE__, 3, *std::next(begin, 3));
  expect(__LINE__, 4, *std::prev(end));
  expect(__LINE__, 2, *std::prev(end, 3));

  auto iter = begin;
  expect(__LINE__, 0, *iter);
  std::advance(iter, 1);
  expect(__LINE__, 1, *iter);
  std::advance(iter, 3);
  expect(__LINE__, 4, *iter);

  std::advance(iter, -2);
  expect(__LINE__, 2, *iter);
  std::advance(iter, 0);
  expect(__LINE__, 2, *iter);

  expect(__LINE__, 3, std::distance(iter, end));
}

// random_access
static void test_advance2_1() {
  array<int, 5> a = {0, 1, 2, 3, 4};

  auto begin = std::begin(a);
  auto end = std::end(a);

  expect(__LINE__, 1, *std::next(begin));
  expect(__LINE__, 3, *std::next(begin, 3));
  expect(__LINE__, 4, *std::prev(end));
  expect(__LINE__, 2, *std::prev(end, 3));

  auto iter = begin;
  expect(__LINE__, 0, *iter);
  std::advance(iter, 1);
  expect(__LINE__, 1, *iter);
  std::advance(iter, 3);
  expect(__LINE__, 4, *iter);

  std::advance(iter, -2);
  expect(__LINE__, 2, *iter);
  std::advance(iter, 0);
  expect(__LINE__, 2, *iter);

  expect(__LINE__, 3, std::distance(iter, end));
}

namespace ns {
// copy instead of print
template <typename InputIterator, typename OutputIterator>
void reverse_copy(InputIterator first, InputIterator last,
                  OutputIterator dest) {
  if (first == last)
    return; // nothing to copy.

  auto iter = std::prev(last);
  while (iter != first)
    *dest++ = *iter--;
  *dest = *iter;
}
} // namespace ns

static void test_reverse_print() {
  std::vector<int> tmp(5);

  std::vector<int> v = {0, 1, 2, 3, 4};
  expect(__LINE__, false, std::begin(v) == std::end(v));
  ns::reverse_copy(std::begin(v), std::end(v), std::begin(tmp));
  expect(__LINE__, 4, tmp[0]);
  expect(__LINE__, 0, tmp[4]);

  std::vector<int> v1 = {1876};
  expect(__LINE__, false, std::begin(v1) == std::end(v1));
  ns::reverse_copy(std::begin(v1), std::end(v1), std::begin(tmp));
  expect(__LINE__, 1876, tmp[0]);

  std::vector<int> v2 = {};
  expect(__LINE__, true, std::begin(v2) == std::end(v2));
  ns::reverse_copy(std::begin(v2), std::end(v2), std::begin(tmp));
  expect(__LINE__, 1876, tmp[0]);
}

static void test_reverse_iterator() {
  std::vector<int> v = {0, 1, 2, 3, 4};

  std::reverse_iterator first{std::end(v)};
  std::reverse_iterator last{std::begin(v)};

  std::vector<int> tmp(v.size());
  std::copy(first, last, std::begin(tmp));
  expect(__LINE__, 4, tmp[0]);
  expect(__LINE__, 0, tmp[4]);

  // cannot compare return value of std::rbegin() and it of std::begin()
  // because these type of object are not same.
  // 	expect(__LINE__, true, std::rbegin(v) == std::begin(v));

  std::vector<int> tmp2(v.size());
  std::reverse_iterator riter{std::end(tmp2)};
  std::copy(std::begin(v), std::end(v), riter);
  expect(__LINE__, 4, tmp2[0]);
  expect(__LINE__, 0, tmp2[4]);
}

static void test_reverse_iterator2() {
  std::vector<int> v = {};
  std::reverse_iterator first{std::end(v)};
  std::reverse_iterator last{std::begin(v)};
  std::vector<int> tmp(v.size());
  std::copy(first, last, std::begin(tmp));
}

static void test_reverse_iterator3() {
  std::vector<int> v = {1916};
  std::reverse_iterator first{std::end(v)};
  std::reverse_iterator last{std::begin(v)};
  std::vector<int> tmp(v.size());
  std::copy(first, last, std::begin(tmp));
  expect(__LINE__, 1916, tmp[0]);
}

static void test_reverse_iterator4() {
  std::vector<int> v = {0, 1, 2, 3, 4};
  std::vector<int> tmp(v.size());

  std::copy(std::rbegin(v), std::rend(v), std::begin(tmp));
  expect(__LINE__, 4, tmp[0]);
  expect(__LINE__, 0, tmp[4]);
}

int main() {

  void test_all_vector();
  void test_all_memory();
  void test_all_error();
  void test_all_reference();
  void test_all_list();
  void test_all_class();
  void test_all_number();
  void test_all_allocator();

  quiet_mode = true;

  test_all_allocator();
  test_all_class();
  test_all_vector();
  test_all_list();
  test_all_error();
  test_all_reference();
  test_all_memory();
  test_all_number();

  test_reverse_iterator4();
  test_reverse_iterator3();
  test_reverse_iterator2();
  test_reverse_iterator();
  test_reverse_print();

  test_advance2_0();
  test_advance2_1();
  test_advance1_0();
  test_advance0_0();
  test_advance0_1();

  ns::test_forward_link_list();
  ns::test_forward_link_list_iterator();

  test_forward_link_list();
  test_forward_link_list_iterator();

  test_iota_iterator();

  if (!quiet_mode) {
    test_print();
    test_cin_iterator();
  }

  test_back_inserter();

  if (!quiet_mode)
    test_cout_iterator();

  test_output_iterator();
  test_output_iterator2();
  test_output_iterator3();

  test_iterator_traits();
  name_scope();
  lambda_expr();
  test_array();
  test_array_iterator();
  test_for_each();
  test_copy();
  test_const();
  test_array_iterator_index();
  test_array_iterator_comparison();
  const_iterator1();
  const_iterator2();
  const_iterator3();
  const_iterator4();
  const_iterator5();
  test_at();
  test_type();

  test_random_access_iter();
  test_random_access_iter2();
  test_random_access_iter3();
  test_random_access_iter4();

  test_bidirectional_iter();
  test_bidirectional_iter3();

  test_forward_iter();
  test_forward_iter3();
  test_forward_iter4();

  if (!quiet_mode) {
    test_input_iter();
    test_input_iter2();

    test_output_iter();
    test_output_iter2();
  }

  test_output_iter3();
  test_output_iter4();

  return EXIT_SUCCESS;
}
// TODO const iterator

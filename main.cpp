void test_all_error();
void test_all_reference();

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

#ifdef ARRAY_ITERATOR1
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

#else
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
#endif
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

template <typename T, std::size_t N>
struct array {

  using iterator = array_iterator<array>;
  using const_iterator = array_const_iterator<array>;
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using const_reference = T const &;

  using size_type = std::size_t;

  value_type storage[N];

  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, N); }

  const_iterator begin() const { return const_iterator(*this, 0); }
  const_iterator end() const { return const_iterator(*this, N); }

  const_iterator cbegin() { return const_iterator(*this, 0); }
  const_iterator cend() { return const_iterator(*this, N); }

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

int main() {
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
  test_at();
  test_type();

  test_all_error();
  test_all_reference();

  return EXIT_SUCCESS;
}

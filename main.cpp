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
  // comment out. refers to issue #1
  // iter = n - i;

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
  static_cast<void>( --iter );
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
  //  using iterator = std::array<int, 5>::iterator;
  using iterator = std::vector<int>::iterator;
  expect(__LINE__, true,
         is_category_of<std::random_access_iterator_tag, iterator>());

  //  std::array<int, 5> a = {0, 1, 2, 3, 4};
  std::vector<int> a = {0, 1, 2, 3, 4};

  auto iter = std::begin(a);
  auto end_iter = std::end(a);

  test_iterator_0(iter, 5);
  test_iterator_1(iter, end_iter);
  test_iterator_1(end_iter, iter);
  test_iterator_3(iter, end_iter);
  test_iterator_5(iter);
  test_iterator_7(iter, end_iter);
  test_iterator_8(iter, 8); // error in std::array<T,N>
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
  test_iterator_7(iter, end_iter);
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

  test_iterator_7(iter, end_iter);
  test_iterator_8(iter, 8);
}

/**
   input iterator
 */
static void test_input_iter() {
  /*
  using iterator = std::XXXXX<int>::iterator;
  expect(__LINE__, false,
         is_category_of<std::forward_iterator_tag, iterator>());
  expect(__LINE__, true,
         is_category_of<std::input_iterator_tag, iterator>());
  */

  //  test_iterator_7(iter, iter);
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

  //
  // cout_iterator
  //
  cout_iterator iter2;
  test_iterator_8(iter2, 8);
}

static void test_random_access_iter3() {
  using iterator = std::vector<int>::iterator;
  expect(__LINE__, true,
         is_category_of<std::random_access_iterator_tag, iterator>());

  std::vector<int> v;
  [[maybe_unused]] int n = 3;

  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  [[maybe_unused]] auto iter = std::begin(v);
  // clang++
  // error: invalid operands to binary expression ('int' and
  // '__gnu_cxx::__normal_iterator<int *, std::vector<int,
  // std::allocator<int> > >')
  //
  // g++
  // error: no match for ‘operator-’ (operand types are ‘int’ and
  // ‘__gnu_cxx::__normal_iterator<int*, std::vector<int> >’)
  //
  // n - iter ;
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

static void test_cout_iterator() {
  std::vector<int> v = {0, 1, 2, 3, 4};
  cout_iterator out;

  std::copy(std::begin(v), std::end(v), out);
}

static void test_back_inserter() {
  std::array<int, 5> a = {0, 1, 2, 3, 4};
  std::vector<int> tmp;
  auto out = back_inserter(tmp);
  //  auto out = std::back_inserter(tmp);
  std::copy(std::begin(a), std::end(a), out);
  expect(__LINE__, 4, tmp[4]);
}

int main() {
  test_back_inserter();
  test_cout_iterator();
  test_output_iterator();
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
  test_random_access_iter3();

  test_bidirectional_iter();

  test_forward_iter();
  test_input_iter();
  test_output_iter();

  test_all_error();
  test_all_reference();

  return EXIT_SUCCESS;
}

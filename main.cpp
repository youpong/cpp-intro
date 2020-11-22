void expect(int line, int expected, int actual);

auto distance = [](auto first, auto last) { return last - first; };

auto equal = [](auto first1, auto last1, auto first2, auto last2) {
  // std::distance()
  if (distance(first1, last1) != distance(first2, last2))
    return false;

  for (auto i = first1, j = first2; i != last1; ++i, ++j)
    if (*i != *j)
      return false;

  return true;
};

auto f = []() { return 1; };

void name_scope() {
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

void lambda_expr() {
  expect(__LINE__, 43, [](auto x) { return x + 1; }(42));
}

// pass std::for_each()
template <typename Array>
struct array_iterator {
  Array &a;
  std::size_t i;

  array_iterator(Array &a, std::size_t i) : a(a), i(i) {}

  long long operator-(array_iterator const &iter) const {
    return i - iter.i;
  }

  array_iterator &operator+=(std::size_t n) {
    i += n;
    return *this;
  }

  array_iterator &operator++() {
    ++i;
    return *this;
  }
  array_iterator operator++(int) {
    array_iterator copy = *this;
    ++*this;
    return copy;
  }

  array_iterator &operator--() {
    --i;
    return *this;
  }
  array_iterator operator--(int) {
    array_iterator copy = *this;
    --*this;
    return copy;
  }

  typename Array::reference operator*() { return a[i]; }

  bool operator==(array_iterator const &right) { return i == right.i; }

  bool operator!=(array_iterator const &right) {
    return !(*this == right);
  }
};

template <typename T, std::size_t N>
struct array {

  using iterator = array_iterator<array>;
  using value_type = T;
  using reference = T &;
  using const_reference = T const &;

  using size_type = std::size_t;

  value_type storage[N];

  iterator begin() { return iterator(*this, 0); }

  iterator end() { return iterator(*this, N); }

  reference operator[](size_type i) { return storage[i]; }

  const_reference operator[](size_type i) const { return storage[i]; }

  reference front() { return storage[0]; }
  const_reference front() const { return storage[0]; }
  reference back() { return storage[N - 1]; }
  const_reference back() const { return storage[N - 1]; }

  void fill(const_reference u) {
    std::fill(std::begin(*this), std::end(*this), u);
  }

  size_type size() const { return N; }
};

void test_array() {
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

  expect(__LINE__, 5, a.size());
  expect(__LINE__, 5, ca.size());

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

void test_array_iterator() {
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

void test_for_each() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  std::for_each(std::begin(a), std::end(a), [](auto &x) { ++x; });

  std::array<int, 5> a0 = {2, 3, 4, 5, 6};
  expect(__LINE__, true,
         equal(std::begin(a), std::end(a), std::begin(a0), std::end(a0)));
}

void expect(int line, int expected, int actual) {
  if (expected == actual)
    return;

  //  error("%d: %d expected, but got %d", line, expected, actual);
  std::cerr << line << ": "s << expected << " expected, but got "s
            << actual << "\n";
}

template <typename Array>
void print(Array const &c) {
  // void print(Array &c) {
  // TODO: implement other way. MUST learn pointer?
  // implements 1:
  //  std::for_each(std::begin(c), std::end(c),
  //		[](auto const &x) { std::cout << x; });
  // std::begin(c): error
  // c.begin(): error
  // array_iterator<Array>(c, 0): error

  // implements 2:
  // for (auto iter = c.begin(); iter != c.end(); ++iter)
  //    std::cout << *iter;

  for (std::size_t i = 0; i != c.size(); ++i)
    std::cout << c[i];
}

template <typename Array>
void copy(Array &dst, Array const &src) {
  for (std::size_t i = 0; i != src.size(); ++i)
    dst[i] = src[i];
}

void test_copy() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  array<int, 5> a_;
  copy(a_, a);
  expect(__LINE__, true,
         equal(std::begin(a), std::end(a), std::begin(a_), std::end(a_)));
  print(a);
}

/*
noreturn void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}
*/

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

void test_const() {
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

void test() {
  array<int, 5> a = {1, 2, 3, 4, 5};

  auto iter = std::begin(a);
  iter += 3;
  expect(__LINE__, 4, *iter);
}

int main() {
  name_scope();
  lambda_expr();
  test_array();
  test_array_iterator();
  test_for_each();
  test_copy();
  test_const();
  test();

  return EXIT_SUCCESS;
}

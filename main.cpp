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

template <typename Array>
struct array_iterator {
  Array &a;
  std::size_t i;

  array_iterator(Array &a, std::size_t i) : a(a), i(i) {}

  long long operator-(array_iterator const &iter) const {
    return i - iter.i;
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

  void fill(const_reference u) { std::fill(begin(), end(), u); }

  size_type size() const { return N; }
};

void myarray() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  auto ref = a[0];
  expect(__LINE__, 1, ref);
  auto size = a.size();
  expect(__LINE__, 5, size);
  int &f = a.front();
  expect(__LINE__, 1, f);

  std::array<int, 5> a0 = {1, 2, 3, 4, 5};
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

int main() {
  name_scope();
  lambda_expr();
  myarray();
  test_array_iterator();
  test_for_each();
  test_copy();
  test_const();

  return EXIT_SUCCESS;
}

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

  array_iterator(Array &a, std::size_t i) : a(a), i(i) {
  }

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

  typename Array::reference operator*() {
    return a[i];
  }

  bool operator==(array_iterator const &right) {
    return i == right.i;
  }

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

  iterator begin() {
    return iterator(*this, 0);
  }
  iterator end() {
    return iterator(*this, N);
  }

  reference operator[](size_type i) {
    return storage[i];
  }

  const_reference operator[](size_type i) const {
    return storage[i];
  }

  reference front() {
    return storage[0];
  }
  const_reference front() const {
    return storage[0];
  }
  reference back() {
    return storage[N - 1];
  }
  const_reference back() const {
    return storage[N - 1];
  }

  void fill(const_reference u) {
    std::fill(begin(), end(), u);
  }

  size_type size() const {
    return N;
  }
};

template <typename Array>
void print(Array const &c) {
  for (std::size_t i = 0; i != c.size(); ++i) {
    std::cout << c[i];
  }
}

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
}

void foo4() {
  array<int, 5> a = {1, 2, 3, 4, 5};

  auto i = a.begin();
  auto j = a.begin();

  std::cout << std::boolalpha << (i == j);
  std::cout << std::boolalpha << (i != ++j);
}

void foo5() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  std::for_each(std::begin(a), std::end(a), [](auto x) { std::cout << x; });
}

void expect(int line, int expected, int actual) {
  if (expected == actual)
    return;

  //  error("%d: %d expected, but got %d", line, expected, actual);
  std::cerr << line << ": "s << expected << " expected, but got "s << actual
            << "\n";
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

int main() {
  name_scope();
  lambda_expr();
  myarray();
  test_array_iterator();
}

/*
noreturn void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}
*/

void expect(int line, int expected, int actual);

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
    for (size_type i = 0; i != N; ++i) {
      storage[i] = u;
    }
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
  auto size = a.size(); // 5
  expect(__LINE__, 5, size);

  int &f = a.front();
  expect(__LINE__, 1, f);

  std::array<int, 5> a_ = {1, 2, 3, 4, 5};
  //  expect(__LINE__, false,
  //  std::equal(std::begin(a), std::end(a), std::begin(a_), std::end(a_));
  std::equal(std::begin(a_), std::end(a_), std::begin(a_), std::end(a_));

  print(a);
  std::cout << "\n"s;
  a.fill(0);
  print(a);
  std::cout << "\n"s;

  std::array<int, 5> a2 = {1, 2, 3, 4, 5};
  int &f2 = a2.front();
  std::cout << f2 << "\n"s;
  print(a2);
  std::cout << "\n"s;
  //  auto first = std::begin(a2);
  auto iter = a2.begin();
  std::cout << *iter;
  *++iter = 0;
  std::cout << a2[1];
}

void foo3() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  auto iter = a.begin();
  std::cout << *iter;
  std::cout << *++iter;
  std::cout << *iter++;
  std::cout << *iter;

  auto iter2 = a.end();

  std::cout << *--iter2;
  std::cout << *iter2--;
  std::cout << *iter2;
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

int main() {
  name_scope();
  lambda_expr();
  myarray();
}

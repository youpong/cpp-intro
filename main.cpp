auto f = []() { std::cout << 1; };

int name_scope() {
  f();

  auto f = []() { std::cout << 2; };

  f();

  {
    f();
    auto f = []() { std::cout << 3; };
    f();
  }

  f();

  return 0;
}

void foo() {
  [](auto x) { std::cout << x; }("hello"s);

  auto answer = 42;
  auto question = "Life, The Universe, and Everything."s;

  auto print = [](auto x) { std::cout << x; };
  print(question);
  print(answer);
  std::cout << "\n";

  for (auto a = 1; a < 10; a++) {
    for (auto b = 1; b < 10; b++)
      std::cout << "\t" << a * b;
    std::cout << "\n"s;
  }

  std::cout << "hello";
}

struct array_iterator {};

template <typename T, std::size_t N>
struct array {

  using iterator = array_iterator;
  using value_type = T;
  using reference = T &;
  using const_reference = T const &;

  using size_type = std::size_t;

  value_type storage[N];

  iterator begin();
  iterator end();

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

struct array_iterator_int_5_begin {
  array<int, 5> &a;
  array_iterator_int_5_begin(array<int, 5> &a) : a(a) {
  }

  array<int, 5>::reference operator*() {
    return a[0];
  }
};

template <typename Array>
void print(Array const &c) {
  for (std::size_t i = 0; i != c.size(); ++i) {
    std::cout << c[i];
  }
}

void foo2() {
  using array_type = array<int, 5>;
  array_type a = {1, 2, 3, 4, 5};
  //  auto ref = a[0];
  //  auto size = a.size(); // 5
  //  std::cout << a.size() << "\n"s;
  int &f = a.front();
  std::cout << f << "\n"s;
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

int main() {
  array<int, 5> a = {1, 2, 3, 4, 5};
  array_iterator_int_5_begin iter(a);
  std::cout << *iter;
}

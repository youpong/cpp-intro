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

template <typename T, std::size_t N>
struct array {

  using value_type = T;
  using reference = T &;
  using const_reference = T const &;

  using size_type = std::size_t;

  value_type storage[N];

  reference operator[](size_type i) {
    return storage[i];
  }

  void fill(const_reference u) {
    for (size_type i = 0; i != N; ++i) {
      storage[i] = u;
    }
  }
};

struct array_iterator {};

int main() {
}

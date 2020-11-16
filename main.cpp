auto f = []() {
  std::cout << 1 ;
 };

int name_scope() {
  f() ;

  auto f = []() {
    std::cout << 2 ;
  };

  f() ;

  {
    f() ;
    auto f = []() {
      std::cout << 3 ;
    };
    f();
  }

  f();

  return 0;
}

int main() {
  [](auto x){ std::cout << x; }("hello"s);
}

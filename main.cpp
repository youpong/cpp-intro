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

  auto answer = 42;
  auto question = "Life, The Universe, and Everything."s ;
  
  auto print = [](auto x) {
		 std::cout << x;
	       } ;
  print(question);
  print(answer);
  std::cout << "\n";
  
  for(auto a = 1; a < 10; a++) {
    for(auto b = 1; b < 10; b++)
      std::cout << "\t" << a * b;
    std::cout << "\n"s ;
  }

  std::cout << "hello";

  return 0;
}

int main() {
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

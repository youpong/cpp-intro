static void test_throw() {
  // throw an instance of 'int'
  // throw 123;

  // throw an instance of 'double'
  // throw 3.14;

  // throw an instance of 'std::array<int, 5ul>'
  // std::array<int, 5> a = {1, 2, 3, 4, 5};
  // throw a;

  // char const*
  // throw "without s";
  // throw an instance of
  // 'std::__cxx11::basic_string<
  //  char, std::char_traits<char>, std::allocator<char> >'
  // throw "with s"s;
}

static void test() {
  std::out_of_range err("I am error."s);
  expect(__LINE__, "I am error."s, err.what());
}

void test_all_error() {
  test_throw();
  test();
}

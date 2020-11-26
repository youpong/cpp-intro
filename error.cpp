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

int f(int);
double g(double, double);

using f_type = int(int);
using g_type = double(double, double);

using f_pointer = f_type *;
using g_pointer = g_type *;

using f_ptr = int (*)(int);

int f(int x) {
  std::cout << x;
  return x;
}

static void test3() {
  //  using f_type
  f_pointer ptr = &f;

  (*ptr)(123);
}

f_ptr g(f_ptr p) {
  p(0);
  return p;
}

static void test2() {
  [[maybe_unused]] int (*(*ptr0)(int (*)(int)))(int) = &g;

  // clang-format off
  [[maybe_unused]]  
  int (*
       (*ptr1)
       (
	int (*)(int)
	)
       )(int) = &g;
  // clang-format on
  [[maybe_unused]] auto (*ptr2)(int (*)(int))->int (*)(int) = &g;
  [[maybe_unused]] auto (*ptr3)(f_ptr)->f_ptr = &g;

  //  ptr0(f);
  //  ptr1(f);
  //  ptr2(f);
  //  ptr3(f);
}

void test_all_error() {
  test_throw();
  test();
  test2();
  test3();
}

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

namespace n {
int f(int);
double g(double, double);
} // namespace n

int n::f(int x) { return x * 2; }

double n::g(double x, double y) { return x * y; }

static void test_func_ptr0() {
  using f_type = int(int);
  using g_type = double(double, double);

  using f_pointer = f_type *;
  using g_pointer = g_type *;

  [[maybe_unused]] f_pointer ptr0 = &n::f;
  [[maybe_unused]] g_pointer ptr1 = &n::g;
}

static void test_func_ptr() {
  auto *ptr0 = &n::f;
  expect(__LINE__, 254, ptr0(127));

  int (*ptr1)(int) = &n::f;
  expect(__LINE__, 250, ptr1(125));

  using function_type = int(int);
  using function_pointer_type = function_type *;

  function_pointer_type ptr2 = &n::f;
  expect(__LINE__, 252, ptr2(126));
}

int gl_v = 0;
int f(int x) {
  ++gl_v;
  return x;
}

using f_ptr = int (*)(int);
f_ptr g(f_ptr p) {
  p(0);
  return p;
}

static void test2() {
  int (*(*ptr0)(int (*)(int)))(int) = &g;

  // clang-format off
  int (*
       (*ptr1)
       (
	int (*)(int)
	)
       )(int) = &g;
  // clang-format on
  auto (*ptr2)(int (*)(int))->int (*)(int) = &g;
  auto (*ptr3)(f_ptr)->f_ptr = &g;

  expect(__LINE__, 0, gl_v);
  ptr0(&f);
  ptr1(&f);
  ptr2(&f);
  ptr3(&f);
  expect(__LINE__, 4, gl_v);  
}

void test_all_error() {
  test_throw();
  test();
  test2();
  test_func_ptr0();
  test_func_ptr();
}

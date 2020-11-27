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
  auto ptr0 = &n::f;
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
  // form: 1
  auto ptr = &g;

  // form: 2
  int (*(*ptr0)(int (*)(int)))(int) = &g;
  // clang-format off
  int (*             // front of return type
      (*ptr1)        // variable name
      (int (*)(int)) // pointer to function as param
  )(int)             // tail of return type
   = &g;             // initializer

  // analyse type below
  // int (* (*ptr0)(int (*)(int)))(int);
  //
  // s1. analyse outside of (*...)
  // = int (*...)(int)
  // -> func(int) int
  // s2. analyse inside of (*...)
  // = * (*ptr0)(int (*)(int))
  //   s2.1. analyse outside of (*...)
  //   -> func(param) *___
  //     s2.1.1. analyse param
  //     param = int (*)(int)
  //       s2.1.1.1 analyse outside of (*)
  //       -> func(int) int
  //       s2.1.1.2 analyse inside of (*)
  //       -> *___
  //       s2.1.1.3 combine
  //       -> * func(int) int
  //     s2.1.2. substitute param
  //     -> func( * func(int) int ) *___
  //   s2.2. analyse inside of (*...)
  //   = *ptr0
  //   -> *___
  //   s2.3. combine
  //   -> * func( * func(int) int ) *___
  // s3. combine
  // -> * func( * func(int) int ) * func(int) int
  // clang-format on

  // form: 3
  auto (*ptr2)(int (*)(int))->int (*)(int) = &g;
  // clang-format off
  auto             // place holder
    (*ptr3)        // variable name
    (int (*)(int)) // params
    ->int (*)(int) // return type
    = &g;          // initializer
  // clang-format on

  // form: 4
  auto (*ptr4)(f_ptr)->f_ptr = &g;

  expect(__LINE__, 0, gl_v);
  ptr(&f);  
  ptr0(&f);
  ptr1(&f);
  ptr2(&f);
  ptr3(&f);
  ptr4(&f);  
  expect(__LINE__, 6, gl_v);  
}

void test_marray() {
  int ma[2][5] = { {1, 2, 3, 4, 5}, { 6, 7, 8, 9, 10} };

  expect(__LINE__, true, &ma[0][0] == ma[0]);
  expect(__LINE__, true, &ma[0] == ma);

  int *p;
  
  p = ma[0]; // equiv. *(ma + 0) or *ma
  expect(__LINE__, 2, *++p); // *++p == ma[0][1]
  
  p = *(ma+1); // equiv. to ma[1]
  expect(__LINE__, 6, *p);  // *p == ma[1][0]
}

void test_all_error() {
  test_throw();
  test_marray();
  test();
  test2();
  test_func_ptr0();
  test_func_ptr();
}

// clang-format off
// Analyse type below.
// void (*signal(int, void (*)(int)))(int);
//
// s1. analyse outside of (*signal...)
// -> func(int) void
// s2. analyse inside of (*signal...)
// = *signal(int, void (*)(int))
// -> func(parm1, param2) *___
//   s2.1. analyse param1
//   param1 -> int
//   $2.2. analyse param2
//   param2 = void (*)(int)
//     s2.2.1. analyse outside of (*)
//     -> func(int) void
//     s2.2.2. analyse inside of (*)
//     -> *___
//     s2.2.3. combine
//     -> * func(int) void
//   s2.3. substitute params
//   -> func(int, * func(int) void) *___
// s3. combine 
// -> func(int, * func(int) void) * func(int) void
// clang-format on

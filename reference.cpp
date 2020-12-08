namespace ns {
void *memcpy(void *dest, const void *src, size_t n);

template <typename To, typename From>
To bit_cast(From const *from);
} // namespace ns

struct S {
  int data;

  int &ref() { return data; }
  int *ptr() { return &data; }
};

void testS() {
  S s;

  *s.ptr() = 2030;
  ++s.ref();

  expect(__LINE__, 2031, s.data);
}

void testS2() {
  S s;
  S *ptr = &s;
  S &ref = *ptr;

  (*ptr).data = 1606822901;
  ptr->data += 1;
  ref.data += 1;

  expect(__LINE__, 1606822903, s.data);
}

int &ref(int &x) { return x; }

int *ptr(int *x) { return x; }

void test() {
  int n = 42;

  ++ref(n);
  expect(__LINE__, 43, n);

  *ptr(&n) = 44;
  expect(__LINE__, 44, n);
}

struct C {
  int data{};

  void set(int n) {
    C *ptr = this;
    ptr->data = n;
  }

  void set2(int n) { this->data = n; }

  int f0() { return data; }
  int f1(int n) { return data + n; }
  int f2(int n, int m) { return data + n + m; }
};

void test_this() {
  C a;
  C b;

  a.set(1606825221);
  b.set2(1606825222);

  expect(__LINE__, 1606825221, a.data);
  expect(__LINE__, 1606825222, b.data);
}

struct CC {
  int data{};
  int f(int n) {
    data += n;
    return data;
  }
};

auto ptr() -> int CC::* { return &CC::data; }

static void test_ptr() {
  CC obj;
  obj.data = 1606913468;
  expect(__LINE__, 1606913468, obj.*ptr());

  // a pointer to member function
  int (CC::*ptr0)(int) = &CC::f;
  expect(__LINE__, 1606913469, (obj.*ptr0)(1));

  CC *obj_ptr = &obj;
  auto data_ptr = &CC::data;
  obj_ptr->*data_ptr += 2;
  expect(__LINE__, 1606913471, obj.data);
}

void test_arrow() {
  auto data_ptr = &C::data;

  C obj;
  auto c_ptr = &obj;

  c_ptr->*data_ptr = 1606997960;
  expect(__LINE__, 1606997960, obj.data);
}

void test_invoke() {
  auto data_ptr = &C::data;

  C obj;
  obj.*data_ptr = 1606998210;
  std::invoke(data_ptr, obj) += 24;

  expect(__LINE__, 1606998234, obj.data);

  C *c_ptr = &obj;
  c_ptr->*data_ptr = 1606998240;
  std::invoke(data_ptr, c_ptr) += 27;
  expect(__LINE__, 1606998267, obj.data);

  obj.data = 1607003750;
  expect(__LINE__, 1607003750, (obj.*&C::f0)());
  expect(__LINE__, 1607003750, std::invoke(&C::f0, obj));

  expect(__LINE__, 1607003778, (obj.*&C::f1)(28));
  expect(__LINE__, 1607003779, std::invoke(&C::f1, obj, 29));

  expect(__LINE__, 1607003811, (obj.*&C::f2)(30, 31));
  expect(__LINE__, 1607003815, std::invoke(&C::f2, obj, 32, 33));
}

template <typename T>
int get_size() {
  return sizeof(T);
}

template <typename To, typename From>
To bit_cast(From const &from) { // int const &from
  To to;
  //  std::cout << from << '\n';
  std::memcpy(&to, &from, sizeof(To));
  return to;
}

template <typename To, typename From>
To ns::bit_cast(From const *from) {
  To to;

  ns::memcpy(&to, &from, sizeof(To));
  return to;
}

void *ns::memcpy(void *dest, const void *src, size_t n) {
  auto *p = static_cast<char *>(dest);
  const auto *q = static_cast<const char *>(src);
  for (size_t i = 0; i < n; ++i)
    *p++ = *q++;

  return dest;
}

// #include <bit>
void test_size() {
  expect(__LINE__, 8, get_size<int *>());
  expect(__LINE__, 8, get_size<double *>());
  expect(__LINE__, 8, get_size<int **>());

  expect(__LINE__, 8, get_size<std::uintptr_t>());

  int data0{};
  std::cout << "&data0:\t" << &data0 << '\n';
  //  std::cout << bit_cast<std::uintptr_t>(&data0) << '\n';
  // std::cout << "&data0:\t" << std::hex <<
  // bit_cast2<std::uintptr_t>(&data0) << '\n';
  auto tmp = ns::bit_cast<std::uintptr_t>(&data0);
  std::cout << "&data0:\t0x" << std::hex << tmp << '\n';

  // std::cout << sizeof(int);
  [[maybe_unused]] std::byte data[4];

  // clang++-10.0.0-4ubuntu1/g++-9.3.0-17ubuntu1(-std=c++2a)
  // error: no member named 'bit_cast' in namespace 'std'
  // std::cout << std::bit_cast<std::uintptr_t>(&data[0]); TODO
  /*
  std::cout << bit_cast<std::uintptr_t>(&data[0]) << '\n';
  std::cout << ns::bit_cast<std::uintptr_t>(&data[0]) << '\n';
  */
}

void test_all_reference() {
  testS();
  testS2();
  test_this();
  test();
  test_ptr();
  test_arrow();
  test_invoke();
  test_size();
}

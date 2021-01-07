#include <stdlib.h>

namespace ns {

void *memcpy(void *dest, const void *src, std::size_t n);

template <typename Dest, typename Src>
Dest *memcpy2(Dest *dest, Src const *src, std::size_t n);

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

/**
 * obtatin value of type To by reinterpreting the object
 * representation of from.
 */
template <typename To, typename From>
To bit_cast(From const &from) {
  To to;
  std::memcpy(&to, &from, sizeof(To));
  return to;
}

template <typename To, typename From>
To ns::bit_cast(From const *from) {
  To to;
  ns::memcpy(&to, &from, sizeof(To));
  return to;
}

void *ns::memcpy(void *dest, void const *src, std::size_t n) {
  auto *p = static_cast<std::byte *>(dest);
  auto *q = static_cast<const std::byte *>(src);

  for (auto last = p + n; p != last; ++p, ++q)
    *p = *q;

  return dest;
}

template <typename Dest, typename Src>
Dest *ns::memcpy2(Dest *dest, Src const *src, std::size_t n) {
  void *void_ptr = static_cast<void *>(dest);
  auto *p = static_cast<std::byte *>(void_ptr);

  void const *const_void_ptr = static_cast<void const *>(src);
  auto *q = static_cast<std::byte const *>(const_void_ptr);

  for (std::size_t i = 0; i != n; ++i)
    p[i] = q[i];

  return dest;
}

// #include <bit>
// clang++-10.0.0-4ubuntu1/g++-9.3.0-17ubuntu1(-std=c++2a)
// error: no member named 'bit_cast' in namespace 'std'
// 	std::bit_cast<std::uintptr_t>(&data[0])
void test_size() {
  expect(__LINE__, 8, get_size<int *>());
  expect(__LINE__, 8, get_size<double *>());
  expect(__LINE__, 8, get_size<int **>());

  expect(__LINE__, 8, get_size<std::uintptr_t>());

  // a address of int_data.
  // int int_data {};
  // bit_cast<std::uintptr_t>(&int_data);

  return;

  expect(__LINE__, 4, get_size<int>());

  std::byte data[sizeof(int)];
  data[0] = static_cast<std::byte>(0xFF);
  for (size_t i = 1; i < sizeof(int); i++)
    data[i] = data[0];

  //  std::cout << bit_cast<std::uintptr_t>(&data[0]);
  std::cout << "bit_cast: " << bit_cast<int>(&data[0]) << '\n';
  std::cout << "ns::bit_cast: " << ns::bit_cast<int>(&data[0]) << '\n';

  int a = 0;
  for (size_t i = 0; i != sizeof(int); i++) {
    //    std::cout << a << '\n';
    a = a * 256 + static_cast<int>(data[i]);
  }
  std::cout << a << '\n';

  return;

  int data0{};
  expect(__LINE__, bit_cast<std::uintptr_t>(&data0),
         ns::bit_cast<std::uintptr_t>(&data0));

  // std::cout << std::bit_cast<std::uintptr_t>(&data[0]); TODO
  /*
  std::cout << bit_cast<std::uintptr_t>(&data[0]) << '\n';
  std::cout << ns::bit_cast<std::uintptr_t>(&data[0]) << '\n';
  */
}

void test_void() {
  // clang++: error: variable has incomplete type 'void'
  // g++:     error: variable or field ‘x’ declared void
  // 	void x;

  expect(__LINE__, 0, 0);
  return static_cast<void>(123);
}

void test_void2() {
  expect(__LINE__, 0, 0);
  return;
}

void test_void_ptr() {
  int data{};

  void *void_ptr = &data;

  [[maybe_unused]] int *int_ptr = static_cast<int *>(void_ptr);

  int const *int_const_ptr = &data;
  void const *void_const_ptr = int_const_ptr;
  [[maybe_unused]] int const *original =
      static_cast<int const *>(void_const_ptr);
}

void test_byte() {
  // error: cannot initialize a variable of type 'std::byte' with an
  // rvalue of type 'int'
  //	std::byte a = 123;

  // error: cannot initialize a variable of type 'std::byte' with an
  // rvalue of type 'int'
  //	std::byte b(123);

  // error: assigning to 'std::byte' from incompatible type 'int'
  //	std::byte c;
  //	c = 123;

  std::byte d{123};
  d = std::byte{234};
}

void test_byte_cast() {
  // error: invalid operands to binary expression ('std::byte' and 'int')
  //	a == 123;

  expect(__LINE__, true, std::byte{123} == static_cast<std::byte>(123));
  expect(__LINE__, true, std::byte{124} == std::byte(124));

  expect(__LINE__, true, std::byte{0} == static_cast<std::byte>(256));
  expect(__LINE__, true, std::byte{0} == std::byte(256));

  expect(__LINE__, true, std::byte{255} == static_cast<std::byte>(-1));
  expect(__LINE__, true, std::byte{255} == std::byte(-1));

  expect(__LINE__, true, std::byte{0} == static_cast<std::byte>(512));
  expect(__LINE__, true, std::byte{0} == std::byte(512));
}

template <typename T>
void print_raw_address(T ptr) {
  // std::cout << std::bit_cast<std::uintptr_t>(ptr) << "\n"s;
  std::cout << bit_cast<std::uintptr_t>(ptr) << "\n"s;
}

void test_addr() {
  int data[] = {0, 1, 2};

  //  print_raw_address(&data[0]);
  //  print_raw_address(&data[1]);
  //  print_raw_address(&data[2]);

  // Two pointers to compatible types may be subtracted. The value of the
  // result is number of array elements that separate the two pointers.
  expect(__LINE__, 1, &data[1] - &data[0]);

  // Subtract two Raw addresses of an object.
  expect(__LINE__, sizeof(int) * 1,
         bit_cast<std::uintptr_t>(&data[1]) -
             bit_cast<std::uintptr_t>(&data[0]));

  expect(__LINE__, sizeof(int) * 2,
         bit_cast<std::uintptr_t>(&data[2]) -
             bit_cast<std::uintptr_t>(&data[0]));
}

void test_ptr_arithmetic() {
  int a[] = {0, 1, 2, 3};

  // a pointer to 0th element.
  int *a0 = &a[0];

  // add sizeof(int) * 3 to a0.
  int *a3 = a0 + 3;
  //  print_raw_address( a3 );
  expect(__LINE__, sizeof(int) * 3,
         bit_cast<std::uintptr_t>(a3) - bit_cast<std::uintptr_t>(a0));

  // subtract sizeof(int) * 2 from a3.
  int *a1 = a3 - 2;
  //  print_raw_address( a1 );
  expect(__LINE__, -sizeof(int) * 2,
         bit_cast<std::uintptr_t>(a1) - bit_cast<std::uintptr_t>(a3));

  // caliculate the address of a[2] with address of a[0]
  // sizeof(std::byte) is 1.
  void *void_ptr = a0;
  std::byte *byte_ptr = static_cast<std::byte *>(void_ptr);
  byte_ptr += sizeof(int) * 2;
  void_ptr = byte_ptr;
  int *a2 = static_cast<int *>(void_ptr);
  expect(__LINE__, 2, *a2);
}

void test_memcpy2() {
  char src[] = "hello";
  char *dest = static_cast<char *>(malloc(strlen(src) + 1));

  char *r = static_cast<char *>(ns::memcpy2(dest, src, strlen(src) + 1));
  expect(__LINE__, "hello"s, dest);
  expect(__LINE__, "hello"s, r);
}

void test_nullptr() {
  int *pointer = nullptr;

  expect(__LINE__, nullptr, pointer);
  ++pointer;
  expect(__LINE__, false, nullptr == pointer);

  int *ptr2 = NULL;
  int *ptr3 = 0;
  expect(__LINE__, nullptr, ptr2);
  expect(__LINE__, nullptr, ptr3);
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
  test_void();
  test_void2();
  test_void_ptr();
  test_byte();
  test_byte_cast();
  test_addr();
  test_ptr_arithmetic();
  test_memcpy2();
  test_nullptr();
}

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

void test_all_reference() {
  testS();
  testS2();
  test_this();
  test();
  test_ptr();
}

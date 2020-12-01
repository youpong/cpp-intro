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

int &ref(int &x) {
  return x;
}

int *ptr(int *x) {
  return x;
}

void test() {
  int n = 42;
  
  ++ref(n);
  expect(__LINE__, 43, n);

  *ptr(&n) = 44;
  expect(__LINE__, 44, n);
}
  
void test_all_reference() {
  testS();
  testS2();  
  test();
}

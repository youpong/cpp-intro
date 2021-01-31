#include "main.h"

static void test_vector() {
  int input{};
  std::vector<int> v;
  while (std::cin >> input) {
    v.push_back(input);
  }
}

static void test_malloc() {
  // malloc
  void *ptr = std::malloc(sizeof(int) * 1);
  if (ptr == nullptr) {
    // fail to allocate memory
    std::abort();
  }

  // type conversion
  int *int_ptr = static_cast<int *>(ptr);

  // assign
  *int_ptr = 200;

  expect(__LINE__, 200, *int_ptr);

  // free
  std::free(ptr);
}

static void test_operator_new() {
  try {
    void *void_ptr = ::operator new(sizeof(int));
    int *int_ptr = static_cast<int *>(void_ptr);
    *int_ptr = 201;
    expect(__LINE__, 201, *int_ptr);
    ::operator delete(void_ptr);
  } catch (std::bad_alloc e) {
    // fail to allocate memory
    std::abort();
  }
}

void test_all_memory() {
  expect(__LINE__, 1, 1);

  if (!quiet_mode)
    test_vector();

  test_malloc();
  test_operator_new();
}

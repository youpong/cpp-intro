#include "main.h"

struct Logger {
  std::string name;

  Logger( std::string name ) : name (name) {
    std::cout << name << " is constructed.\n"s;
  }
  ~Logger() {
    std::cout << name << " is destructed.\n"s;
  }
};

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

static void test_Logger() {
  void * raw_ptr = ::operator new ( sizeof(Logger) );
  // new (RAW_PTR) TYPE NEW_INITIALIZER
  // NEW_INITIALIZER = ( params )
  //                 = { params }   
  Logger * logger_ptr = new (raw_ptr) Logger{"Alice"s};
  logger_ptr->~Logger();
  ::operator delete(raw_ptr);
}

static void test_new() {
  int *int_ptr = new int{202};
  expect(__LINE__, 202, *int_ptr);
  delete int_ptr;
  
  
  std::vector<int> *vector_ptr = new std::vector<int>{203, 204};
  expect(__LINE__, 204, (*vector_ptr)[1]);
  delete vector_ptr;
}

static void test_new_array() {
  int int_array[] = {0, 1, 2, 3, 4};
  int * int_array_ptr = new int[]{0, 1, 2, 3, 4};
  
  int * temp = new int[5];
  
  for(std::size_t i = 0; i != 5; i++)
    temp[i] = int_array_ptr[i];
      
  expect(__LINE__, 0, temp[0]);
  expect(__LINE__, 4, temp[4]);
  expect(__LINE__, 5, sizeof(int_array)/sizeof(int));
  // TODO: how to get size of array is pointerd by int_array_ptr.
  // 	sizeof(*int_array_ptr) == sizeof(int)

  delete[] int_array_ptr;
}

void test_all_memory() {
  expect(__LINE__, 1, 1);

  if (!quiet_mode)
    test_vector();

  test_malloc();
  test_operator_new();
  if (!quiet_mode)
    test_Logger();
  test_new();
  test_new_array();
}

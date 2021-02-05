#include "main.h"

//#define ACCESS_MOD

struct Dog {
  int age;

private:
};

class Cat {
  int age;

public:
  void set_age(int age) { this->age = age; }
  int get_age() { return age; }

private:
  std::string name;
};

static void test_access_modifier_class() {
  Cat cat;
  cat.set_age(77);
  expect(__LINE__, 77, cat.get_age());
#ifdef ACCESS_MOD
  // error: 'age' is a private member of 'Cat'
  cat.age = 78;
#endif
}

static void test_access_modifier_struct() {
  //  Dog
}

void test_all_class() {
  test_access_modifier_class();
  test_access_modifier_struct();
}

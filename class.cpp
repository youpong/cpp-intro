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
  void set_weight(double weight) {
    // can set before declare
    this->weight = weight;
  }
  double get_weight() { return weight; }

private:
  double weight; // kg
};

static void test_access_modifier_class() {
  Cat cat;
  cat.set_age(77);
  expect(__LINE__, 77, cat.get_age());
  cat.set_weight(7.8);
  expect(__LINE__, 7.8, cat.get_weight());
#ifdef ACCESS_MOD
  // error: 'age' is a private member of 'Cat'
  cat.age = 78;
  cat.weight = 7.8;
#endif
}

static void test_access_modifier_struct() {
  //  Dog dog;
}

void test_all_class() {
  test_access_modifier_class();
  test_access_modifier_struct();
}

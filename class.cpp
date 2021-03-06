#include "main.h"

//#define ACCESS_MOD

struct Dog {
  // default access modifier is 'public'
  Dog(int age) : age(age) {}

  int get_age() { return age; }

private:
  int age;
};

class Cat {
  // default access modifier is 'private'
  int age;

public:
  Cat(int age, double weight) : age(age), weight(weight) {}

  int get_age() { return age; }
  double get_weight() { return weight; }

private:
  double weight; // kg
};

static void test_access_modifier_class() {
  Cat cat(77, 7.8);
  expect(__LINE__, 77, cat.get_age());
  expect(__LINE__, 7.8, cat.get_weight());
#ifdef ACCESS_MOD
  // error: 'age' is a private member of 'Cat'
  cat.age = 78;
  cat.weight = 7.8;
#endif
}

static void test_access_modifier_struct() {
  Dog dog(79);
  expect(__LINE__, 79, dog.get_age());
#ifdef ACCESS_MOD
  // error: 'age' is a private member of 'Cat'
  dog.age = 79;
#endif
}

void test_all_class() {
  test_access_modifier_class();
  test_access_modifier_struct();
}

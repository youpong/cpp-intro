#include "main.h"

//#define EXPLICIT

class Number {
public:
  double num;
  //
  // copy constructors
  //
#ifdef EXPLICIT
  explicit Number(int i) { num = i; }
  explicit Number(double d) : num(d) {}
#else
  Number(int i) { num = i; }
  Number(double d) : num(d) {}
#endif
  Number(std::string s) {
    // TODO: implement
    num = 0;
  }
};

void print_number(Number n) { std::cout << n.num; }

Number pi() {
#ifdef EXPLICIT
  return Number(3.1415926);
#else
  return 3.1415926;
#endif
}

static void test_print_number() {
#ifdef EXPLICIT
  print_number(Number(1));
  print_number(Number(1.7));
#else
  print_number(1);
  print_number(1.7);
#endif
  print_number("2.1"s);
}

static void test_pi() { expect(__LINE__, 3.1415926, pi().num); }

void test_all_number() {
  if (!quiet_mode)
    test_print_number();
  test_pi();
}

#include "util.h"
#include <setjmp.h>

static jmp_buf g_env;
static int g;

static void func() {
  ++g;
  longjmp(g_env, 1);
}

static void func_caller() {
  int x = 10;
  volatile int y = 20;
  g = 100;
  
  switch (setjmp(g_env)) {
  case 0:
    ++y;
    ++g;
    func();    
    break;
  case 1:
    expect(__LINE__, 10, x);
    expect(__LINE__, 21, y);
    expect(__LINE__, 102, g);
    return;
  }
}

static void func_caller1() {
  g = 100;
  
  switch (setjmp(g_env)) {
  case 0:
    func();
    break;
  case 1:
    expect(__LINE__, 101, g);
    break;
  }

  switch (setjmp(g_env)) {
  case 0:
    func();
    break;
  case 1:
    expect(__LINE__, 102, g);
    break;
  }
}

void test_all_setjmp() {
  func_caller();
  func_caller1();
}

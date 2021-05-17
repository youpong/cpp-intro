#include "util.h"
#include <setjmp.h>
//#include <stdio.h>

jmp_buf g_env;
int g = 100;

static void func() {
  g++;
  longjmp(g_env, 1);
}

static void func_caller() {
  int x = 10;
  volatile int y = 20;

  switch (setjmp(g_env)) {
  case 0:
    y++;
    g++;
    break;
  case 1:
    expect(__LINE__, 10, x);
    expect(__LINE__, 21, y);
    expect(__LINE__, 102, g);
    return;
  }

  func();
}

void test_all_setjmp() { func_caller(); }

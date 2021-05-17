#include "util.h"
#include <setjmp.h>
#include <stdio.h>

jmp_buf g_env;
int g = 100;

static void func() {
  g *= 2;
  longjmp(g_env, 1);
}

static void func_caller() {
  int x = 10;
  volatile int y = 20;

  if (setjmp(g_env) == 0) {
    y *= 2;
    g *= 2;
    func();
  } else {
    // longjmp()
    printf("x == %d, y == %d, g == %d\n", x, y, g);
  }
}

void test_all_setjmp() { func_caller(); }

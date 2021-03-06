#include "util.h"
#include <setjmp.h>

static int g;

/* use global jmp_buf */

static jmp_buf g_env;

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

/* use local jmp_buf */

static void lfunc(jmp_buf env) {
  ++g;
  longjmp(env, 42);
}

static void lfunc_caller() {
  jmp_buf l_env;

  g = 65;
  switch (setjmp(l_env)) {
  case 0:
    lfunc(l_env);
    break;
  case 42:
    expect(__LINE__, 66, g);
    break;
  }
}

void test_all_setjmp() {
  func_caller();
  func_caller1();
  lfunc_caller();
}

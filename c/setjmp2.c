#include "util.h"
#include <setjmp.h>
#include <stdlib.h>

static void func(jmp_buf jbuf, int val) { longjmp(jbuf, val); }

static void func_caller() {
  jmp_buf env[2];
  char trace[255];
  char *p;

  p = trace;
  switch (setjmp(env[0])) { // setjmp0
  case 0:
    *p++ = 'A';
    func(env[0], 1); // goto setjmp0
    break;
  case 1:
    *p++ = 'B';
    break;
  case 2:
    *p++ = 'C';
    func(env[1], 3); // goto setjmp1
    break;
  }

  switch (setjmp(env[1])) { // setjmp1
  case 0:
    *p++ = 'D';
    func(env[0], 2); // goto setjmp0
    break;
  case 3:
    *p++ = 'E';
    break;
  }

  *p = '\0';

  expect_str(__LINE__, trace, "ABDCE");
}

void test_all_setjmp2() { func_caller(); }

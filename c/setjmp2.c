#include "util.h"
#include <setjmp.h>
#include <stdlib.h>

// static int g = 56;

static void func(jmp_buf jbuf, int val) {
  //++g;
  longjmp(jbuf, val);
}

static void func_caller() {
  jmp_buf env[2];
  int flg = 0;

  // clang-format off
  // execute A->B->C->D->E
  switch (setjmp(env[0])) { // setjmp0
  case 0:                   // A
    flg |= 0b1; 
    func(env[0], 1);        // goto setjmp0
    break;
  case 1:                   // B
    flg |= 0b10;
    break;
  case 2:                   // D
    flg |= 0b100;
    func(env[1], 3);        // goto setjmp1
    break;
  }

  switch (setjmp(env[1])) { // setjmp1
  case 0:                   // C
    flg |= 0b1000;
    func(env[0], 2);        // goto setjmp0
    break;
  case 3:                   // E
    flg |= 0b10000;
    break;
  }
  // clang-format on  

  expect(__LINE__, flg, 0b11111);
}

void test_all_setjmp2() { func_caller(); }

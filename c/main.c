#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void test_all_memcpy();
void test_all_malloc();
void test_all_setjmp();
void test_all_ptr();

int main(int argc, char **argv) {
  test_all_malloc();
  test_all_memcpy();
  test_all_setjmp();
  test_all_ptr();

  fputs("okay\n", stdout);

  return EXIT_SUCCESS;
}

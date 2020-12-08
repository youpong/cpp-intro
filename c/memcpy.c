// size_t can be further imported by inclusion of stdlib.h
#include "util.h"
#include <stdlib.h>
#include <string.h>

void *memcpy_(void *dest, const void *src, size_t n) {
  char *p = dest;
  const char *q = src;
  for (size_t i = 0; i < n; ++i)
    *p++ = *q++;

  return dest;
}

static void test_memcpy_() {
#define MSG "HELLO"
  char *src = MSG;
  char dest[sizeof MSG];
  memcpy_(dest, src, strlen(src) + 1);

  expect_str(__LINE__, "HELLO", dest);
}

static void test_memcpy_int() {
  int src = 1607414210;
  int dest;

  memcpy_(&dest, &src, sizeof(src));

  expect(__LINE__, 1607414210, dest);
}

void test_all_memcpy() {
  test_memcpy_();
  test_memcpy_int();
}

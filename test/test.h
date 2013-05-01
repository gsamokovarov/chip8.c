#ifndef _TEST_H
#define _TEST_H

#include <assert.h>

#define SUITE(name) test_t ##name_test_suite[]
#define TEST(suite, name) void test_##suite_##name(void)
#define TEST_REF(name) test_##name
#define RUN_TEST_SUITE(suite) do { \
  int i;                           \
                                   \
  i = 0;                           \
  while (suite[i++]) {             \
    suite[i - 1]();                \
  }                                \
} while (0)

typedef void (* test_t)(void);

#endif

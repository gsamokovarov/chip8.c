#ifndef _TEST_H
#define _TEST_H

#include <assert.h>

#define TEST(name) void test_##name(void)
#define UNIT(name) test_##name
#define RUN_TEST_SUITE(suite) do {                         \
  int i, len;                                              \
                                                           \
  for (i = 0; i < sizeof(suite) / sizeof(suite[0]); i++) { \
    suite[i]();                                            \
  }                                                        \
} while (0)

typedef void (* test_t)(void);

#endif

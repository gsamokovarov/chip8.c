#include "test.h"
#include "test_chip8.c"

int main(void) {
  RUN_TEST_SUITE(chip8_test_suite);
  return 0;
}

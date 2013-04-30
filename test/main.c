#include "test.h"
#include "test_app.c"
#include "test_chip8.c"

int main(void) {
  RUN_TEST_SUITE(chip8_test_suite);
  RUN_TEST_SUITE(app_test_suite);

  return 0;
}

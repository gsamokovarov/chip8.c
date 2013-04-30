#include "test.h"
#include "app.h"

TEST(app_new) {
  app_t * app = app_new();

  assert(app->running == 0);
  assert(app->io      == 0);

  app_free(app);
}

static test_t app_test_suite[] = {
  TEST_REF(app_new),
  0
};

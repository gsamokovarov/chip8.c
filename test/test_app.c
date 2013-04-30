#include "test.h"
#include "app.h"

TEST(app_new) {
  app_t * app = app_new();

  assert(app->running == 0);
  assert(app->io      == 0);

  app_free(app);
}

TEST(current_app_set_to) {
  app_t * app = app_new();

  assert(current_app == 0);

  current_app_set_to(app);

  assert(current_app == app);

  app_free(app);
  current_app_set_to(0);
}

static test_t app_test_suite[] = {
  TEST_REF(app_new),
  0
};

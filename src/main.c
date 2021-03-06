#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "chip8.h"
#include "io.h"
#include "io/terminal_io.h"
#include "io/ncurses_io.h"
#include "io/sdl_io.h"
#include "app.h"

int main(int argc, char ** argv) {
  app_t * app = app_new();

  if (!app) {
    fprintf(stderr, "Can not allocate app at %d\n", __LINE__);
    goto error;
  }

  current_app_set_to(app);
  current_app_register_signal_handlers();

  if (!app_parse_command_line(app, argc, argv)) goto error;
  if (!app_setup(app)) goto error;

  app_run(app);
  app_teardown(app);
  app_free(app);

  return EXIT_SUCCESS;

error:
  app_free(app);

  return EXIT_FAILURE;
}

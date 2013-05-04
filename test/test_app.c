#include <stdio.h>
#include <string.h>
#include "test.h"
#include "io.h"
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

  assert(current_app == 0);
}

TEST(app_parse_command_line_terminal) {
  app_t   * app = app_new();
  int      argc = 2;
  char * argv[] = {"chip8", "--terminal", "roms/INVADERS"};

  app_parse_command_line(app, argc, argv);
  assert(app->io && strcmp(app->io->name, "terminal") == 0);

  app_free(app);
}

TEST(app_parse_command_line_ncurses) {
  app_t   * app = app_new();
  int      argc = 2;
  char * argv[] = {"chip8", "--ncurses", "roms/INVADERS"};

  app_parse_command_line(app, argc, argv);
  assert(app->io && strcmp(app->io->name, "ncurses") == 0);

  app_free(app);
}

TEST(app_parse_command_line_sdl) {
  app_t   * app = app_new();
  int      argc = 2;
  char * argv[] = {"chip8", "--sdl", "roms/INVADERS"};

  app_parse_command_line(app, argc, argv);
  assert(app->io && strcmp(app->io->name, "sdl") == 0);

  app_free(app);
}

TEST(app_parse_command_line_filename) {
  app_t   * app = app_new();
  int      argc = 2;
  char * argv[] = {"chip8", "roms/INVADERS"};

  app_parse_command_line(app, argc, argv);
  assert(strcmp(app->io->name, "sdl") == 0);
  assert(strcmp(app->filename, "roms/INVADERS") == 0);

  app_free(app);
}

static test_t app_test_suite[] = {
  UNIT(app_new),
  UNIT(current_app_set_to),
  UNIT(app_parse_command_line_terminal),
  UNIT(app_parse_command_line_ncurses),
  UNIT(app_parse_command_line_sdl),
  UNIT(app_parse_command_line_filename)
};

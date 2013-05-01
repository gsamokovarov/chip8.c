#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include "chip8.h"
#include "io.h"
#include "app.h"

app_t * current_app = 0;

app_t * app_new(void) {
  app_t * self = (app_t *) malloc(sizeof(app_t));

  self->running = 0;
  self->io      = 0;
  self->chip8   = chip8_new();

  return self;
}

void app_run(app_t * self) {
  if (self->io && self->chip8) {
    self->running = 1;
    while (self->running) {
      if (io_listen(self->io, self->chip8)) {
        break;
      };
      chip8_tick(self->chip8);
      io_render(self->io, self->chip8);
      io_beep(self->io, self->chip8);
      io_delay(self->io, self->chip8);
    }
  } else {
#if CHIP8_DEBUG
    fprintf(stderr, "Trying to run an app with a NULL io or chip8\n");
#endif
  }
}

void app_setup(app_t * self) {
  if (self->io) {
    io_setup(self->io);
  }
}

void app_parse_command_line(app_t * self, int argc, char ** argv) {
  struct option options[] = {
    {"io", optional_argument, 0, 'i'},
    {0, 0, 0, 0}
  };
}

void app_teardown(app_t * self) {
  if (self->io) {
    io_teardown(self->io);
  }
}

void app_free(app_t * self) {
  if (self->chip8) {
    chip8_free(self->chip8);
  }
  if (self->io) {
    io_free(self->io);
  }
  if (current_app == self) {
    current_app_set_to(0);
  }
  free(self);
}

void current_app_set_to(app_t * app) {
  current_app = app;
}

void current_app_setup_signal_handlers(void) {
  if (current_app) {
    signal(SIGINT, current_app_handle_interrupt);
  } else {
#if CHIP8_DEBUG
    fprintf(stderr, "Trying to setup signal handlers on NULL current_app\n");
#endif
  }
}

void current_app_handle_interrupt(int signal) {
  UNUSED(signal);

  if (current_app) {
    current_app->running = 0;
  } else {
#if CHIP8_DEBUG
    fprintf(stderr, "Trying to stop a NULL current_app\n");
#endif
  }
}

#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include "app.h"
#include "io.h"

void app_parse_command_line(app_t * self, int argc, char ** argv) {
  struct option options[] = {
    {"io", optional_argument, 0, 'i'},
    {0, 0, 0, 0}
  };
}

void current_app_setup_signal_handlers(void) {
  if (current_app) {
    signal(SIGINT, current_app_handle_interrupt);
  } else {
#if CHIP8_DEBUG
    fprintf(stderr, "Trying to setup signal handlers on NULL current_app");
#endif
  }
}

void current_app_handle_interrupt(int signal) {
  UNUSED(signal);

  if (current_app) {
    current_app->running = 0;
  } else {
#if CHIP8_DEBUG
    fprintf(stderr, "Trying to stop a NULL current_app");
#endif
  }
}

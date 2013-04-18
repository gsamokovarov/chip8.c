#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "chip8.h"
#include "io.h"
#include "io/terminal_io.h"
#include "io/sdl_io.h"

void handle_interrupt(int);
int chip8_is_running = 1;

int main(int argc, char ** argv) {
  chip8_t * chip8 = chip8_new();
  io_t * io = sdl_io_new();

  if (argc == 2) {
    if (!chip8_load_file(chip8, argv[1])) {
      goto error;
    };
    signal(SIGINT, handle_interrupt);
    io_setup(io);
    while (chip8_is_running) {
      io_listen(io, chip8);
      chip8_tick(chip8);
      io_render(io, chip8);
      usleep(167);
    }
  } else {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
  }

  io_teardown(io);
  chip8_free(chip8);
  io_free(io);
  return 0;

error:
  io_teardown(io);
  chip8_free(chip8);
  io_free(io);
  return -1;
}

void handle_interrupt(int signal) {
  UNUSED(signal);

  chip8_is_running = 0;
}

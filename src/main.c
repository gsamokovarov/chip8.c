#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "chip8.h"
#include "io.h"
#include "io/terminal_io.h"
#include "io/sdl_io.h"

void handle_interrupt(int);
volatile sig_atomic_t chip8_isnt_interrupted = 1;

int main(int argc, char ** argv) {
  chip8_t * chip8 = chip8_new();
  io_t    * io    = 0;

  if (argc == 3) {
    if (!chip8_load_file(chip8, argv[2])) {
      goto error;
    };
    if (strcmp(argv[1], "sdl") == 0) {
      io = sdl_io_new();
    } else if (strcmp(argv[1], "terminal") == 0) {
      io = terminal_io_new();
    } else {
      fprintf(stderr, "Invalid io, use sdl or terminal\n");
      goto error;
    }
    signal(SIGINT, handle_interrupt);
    io_setup(io);
    while (chip8_isnt_interrupted) {
      if (!io_listen(io, chip8)) {
        break;
      };
      chip8_tick(chip8);
      io_render(io, chip8);
      io_beep(io, chip8);
      io_delay(io, chip8);
    }
    io_teardown(io);
  } else {
    fprintf(stderr, "Usage: %s <sdl|terminal> <filename>\n", argv[0]);
  }

  chip8_free(chip8);
  if (io) {
    io_free(io);
  }

  return 0;

error:
  chip8_free(chip8);
  if (io) {
    io_free(io);
  }

  return 1;
}

void handle_interrupt(int signal) {
  UNUSED(signal);

  chip8_isnt_interrupted = 0;
}

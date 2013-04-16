#include <stdio.h>
#include <unistd.h>
#include "chip8.h"
#include "io.h"
#include "io/terminal_io.h"

int main(int argc, char ** argv) {
  chip8_t * chip8 = chip8_new();
  io_t * io = terminal_io_new();

  if (argc == 2) {
    if (!chip8_load_file(chip8, argv[1])) {
      goto error;
    };
    setvbuf(stdout, 0, _IOFBF, 0);
    while (1) {
      chip8_tick(chip8);
      io->render(io, chip8);
      usleep(16667);
      fflush(stdout);
    }
  } else {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
  }

  chip8_free(chip8);
  io_free(io);
  return 0;
error:
  chip8_free(chip8);
  io_free(io);
  return -1;
}

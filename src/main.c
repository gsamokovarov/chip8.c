#include <stdio.h>
#include <unistd.h>
#include "chip8.h"

int main(int argc, char ** argv) {
  chip8_t * chip8 = chip8_new();

  if (argc == 2) {
    if (!chip8_load_file(chip8, argv[1])) {
      goto error;
    };
    setvbuf(stdout, 0, _IOFBF, 4096);
    while (1) {
      chip8_tick(chip8);
      chip8_render_to_terminal(chip8);
      usleep(16667);
      fflush(stdout);
    }
  } else {
    fprintf(stderr, "Usage %s filename\n", argv[0]);
  }

  chip8_free(chip8);
  return 0;
error:
  chip8_free(chip8);
  return -1;
}

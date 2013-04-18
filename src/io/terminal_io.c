#include <stdlib.h>
#include <stdio.h>
#include "../io.h"
#include "../chip8.h"
#include "terminal_io.h"

io_t * terminal_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->setup    = &terminal_io_setup;
  self->render   = &terminal_io_render;
  self->listen   = &terminal_io_listen;
  self->teardown = &terminal_io_teardown;
  self->custom   = 0;

  return self;
}

void terminal_io_setup(io_t * self) {
  UNUSED(self);

  setvbuf(stdout, 0, _IOFBF, 0);
  printf("\033[?25l");
}

void terminal_io_render(io_t * self, chip8_t * chip8) {
  UNUSED(self);

  unsigned char i, j;

  for (j = 0; j < 32; j++) {
    for (i = 0; i < 64; i++) {
      printf(chip8->screen[32 * i + j] ? "█" : " ");
    }
    printf("\n");
  }
  printf("\033[32A");
  fflush(stdout);
}

void terminal_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

void terminal_io_teardown(io_t * self) {
  UNUSED(self);

  printf("\033[?25h");
}

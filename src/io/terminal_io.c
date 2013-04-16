#include <stdlib.h>
#include <stdio.h>
#include "../io.h"
#include "../chip8.h"
#include "terminal_io.h"

io_t * terminal_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->render = &terminal_io_render;
  self->listen = &terminal_io_listen;

  return self;
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
}

void terminal_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

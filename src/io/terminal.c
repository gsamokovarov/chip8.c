#include <stdlib.h>
#include <stdio.h>
#include "../io.h"
#include "../chip8.h"
#include "terminal.h"

io_t * terminalio_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->render = &terminalio_render;
  self->listen = &terminalio_listen;

  return self;
}

void terminalio_render(io_t * self, chip8_t * chip8) {
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

void terminalio_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

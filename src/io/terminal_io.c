#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "io.h"
#include "chip8.h"
#include "io/terminal_io.h"

io_t * terminal_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->name     = "terminal";
  self->setup    = &terminal_io_setup;
  self->render   = &terminal_io_render;
  self->beep     = &terminal_io_beep;
  self->listen   = &terminal_io_listen;
  self->teardown = &terminal_io_teardown;
  self->delay    = &terminal_io_delay;
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

void terminal_io_beep(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  if (ST(chip8) == 1) {
    printf("\a");
    printf("\033[1D");
  }
}

int terminal_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  return 1;
}

void terminal_io_delay(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  usleep(167);
}

void terminal_io_teardown(io_t * self) {
  UNUSED(self);

  printf("\033[?25h");
}

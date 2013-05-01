#include <stdlib.h>
#include <stdio.h>
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
  self->delay    = &terminal_io_delay;
  self->teardown = &terminal_io_teardown;
  self->custom   = 0;

  return self;
}

void terminal_io_setup(io_t * self) {
  UNUSED(self);
}

void terminal_io_render(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

void terminal_io_beep(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

int terminal_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  return 1;
}

void terminal_io_delay(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

void terminal_io_teardown(io_t * self) {
  UNUSED(self);
}

#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "chip8.h"
#include "io/ncurses_io.h"

io_t * ncurses_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->name     = "ncurses";
  self->setup    = &ncurses_io_setup;
  self->render   = &ncurses_io_render;
  self->beep     = &ncurses_io_beep;
  self->listen   = &ncurses_io_listen;
  self->delay    = &ncurses_io_delay;
  self->teardown = &ncurses_io_teardown;
  self->custom   = 0;

  return self;
}

void ncurses_io_setup(io_t * self) {
  UNUSED(self);
}

void ncurses_io_render(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

void ncurses_io_beep(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

int ncurses_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  return 1;
}

void ncurses_io_delay(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

void ncurses_io_teardown(io_t * self) {
  UNUSED(self);
}

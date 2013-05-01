#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "chip8.h"
#include "io/sdl_io.h"

io_t * sdl_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->name     = "sdl";
  self->setup    = &sdl_io_setup;
  self->render   = &sdl_io_render;
  self->beep     = &sdl_io_beep;
  self->listen   = &sdl_io_listen;
  self->delay    = &sdl_io_delay;
  self->teardown = &sdl_io_teardown;
  self->custom   = malloc(sizeof(sdl_io_custom_t));

  return self;
}

void sdl_io_setup(io_t * self) {
  UNUSED(self);
}

void sdl_io_render(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

void sdl_io_beep(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

int sdl_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  return 1;
}

void sdl_io_delay(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

void sdl_io_teardown(io_t * self) {
  UNUSED(self);
}

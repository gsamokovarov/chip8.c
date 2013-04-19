#include <stdlib.h>
#include "io.h"

void io_setup(io_t * self) {
  self->setup(self);
}

void io_render(io_t * self, chip8_t * chip8) {
  self->render(self, chip8);
}

int io_listen(io_t * self, chip8_t * chip8) {
  return self->listen(self, chip8);
}

void io_delay(io_t * self, chip8_t * chip8) {
  self->delay(self, chip8);
}

void io_teardown(io_t * self) {
  self->teardown(self);
}

void io_free(io_t * self) {
  if (self->custom) {
    free(self->custom);
  }
  free(self);
}

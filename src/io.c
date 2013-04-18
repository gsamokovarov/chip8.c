#include <stdlib.h>
#include "io.h"

void io_setup(io_t * self) {
  self->setup(self);
}

void io_render(io_t * self, chip8_t * chip8) {
  self->render(self, chip8);
}

void io_listen(io_t * self, chip8_t * chip8) {
  self->listen(self, chip8);
}

void io_teardown(io_t * self) {
  self->teardown(self);
}

void io_free(io_t * self) {
  free(self);
}

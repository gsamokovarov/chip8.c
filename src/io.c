#include <stdlib.h>
#include "io.h"

inline void io_setup(io_t * self) {
  self->setup(self);
}

inline void io_render(io_t * self, chip8_t * chip8) {
  self->render(self, chip8);
}

inline void io_beep(io_t * self, chip8_t * chip8) {
  self->beep(self, chip8);
}

inline int io_listen(io_t * self, chip8_t * chip8) {
  return self->listen(self, chip8);
}

inline void io_delay(io_t * self, chip8_t * chip8) {
  self->delay(self, chip8);
}

inline void io_teardown(io_t * self) {
  self->teardown(self);
}

inline void io_free(io_t * self) {
  if (self->custom) {
    free(self->custom);
  }
  free(self);
}

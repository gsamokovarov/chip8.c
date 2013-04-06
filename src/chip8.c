#include <stdlib.h>
#include <string.h>
#include "chip8.h"

chip8_t * chip8_new(void) {
  chip8_t * self = (chip8_t *) malloc(sizeof(chip8_t));

  // The first 512 bytes are used by the interpreter.
  self->program_counter = 0x200;
  self->index_register  = 0;
  self->stack_pointer   = 0;
  self->opcode          = 0;

  memcpy(self->memory, chip8_hex_font, 50);

  return self;
}

void chip8_fetch_opcode(chip8_t * self) {
  self->opcode = self->memory[self->program_counter] << 8 |
                 self->memory[self->program_counter + 1];
}

void chip8_free(chip8_t * self) {
  free(self);
}

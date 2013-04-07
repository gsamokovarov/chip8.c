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

void chip8_fetch_current_opcode(chip8_t * self) {
  self->opcode = self->memory[self->program_counter] << 8 |
                 self->memory[self->program_counter + 1];
}

void chip8_next_opcode(chip8_t * self) {
  self->program_counter += 2;
}

void chip8_decode_current_opcode(chip8_t * self) {
  switch (self->opcode & 0xF000) {
  case 0x0000: // 0x00E0
    if (self->opcode & 0x00F0 == 0x00E0 & 0x00F0) {
      memset(self->memory, 0, 64 * 32);
    }
    break;
  case 0xA000:
    self->index_register = self->opcode & 0x0FFF;
    chip8_next_opcode(self);
    break;
  }
}

void chip8_free(chip8_t * self) {
  free(self);
}

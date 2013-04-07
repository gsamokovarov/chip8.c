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

void chip8_skip_next_opcode(chip8_t * self) {
  self->program_counter += 4;
}

void chip8_decode_current_opcode(chip8_t * self) {
  switch (self->opcode & 0xF000) {
  case 0x0000:
    if (self->opcode == 0x00E0) {
      memset(self->memory, 0, 64 * 32);
      break;
    }
    if (self->opcode == 0x00EE) {
      self->program_counter = self->stack[self->stack_pointer--];
      break;
    }
    break;
  case 0x1000:
    self->program_counter = self->opcode & 0x0FFF;
    break;
  case 0x2000:
    self->stack[++self->stack_pointer] = self->program_counter;
    self->program_counter              = self->opcode & 0x0FFF;
    break;
  case 0x3000:
  case 0x4000:
    if (self->registers[(self->opcode & 0x0F00) >> 8] ==
        (self->opcode & 0x00FF)) {
      (self->opcode & 0xF000) == 0x3000
        ? chip8_skip_next_opcode(self)
        : chip8_next_opcode(self);
    } else {
      (self->opcode & 0xF000) == 0x4000
        ? chip8_skip_next_opcode(self)
        : chip8_next_opcode(self);
    }
    break;
  case 0x5000:
    if (self->registers[(self->opcode & 0x0F00) >> 8] ==
        self->registers[(self->opcode & 0x00F0) >> 4]) {
      chip8_skip_next_opcode(self);
    } else {
      chip8_next_opcode(self);
    }
    break;
  case 0x6000:
    self->registers[(self->opcode & 0x0F00) >> 8] = self->opcode & 0x00FF;
    chip8_next_opcode(self);
    break;
  case 0x7000:
    self->registers[(self->opcode & 0x0F00) >> 8] += self->opcode & 0x00FF;
    chip8_next_opcode(self);
    break;
  case 0x8000:
    switch (self->opcode & 0x000F) {
    case 0:
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x00F0) >> 4];
      break;
    case 0x0001:
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x0F00) >> 8] |
        self->registers[(self->opcode & 0x00F0) >> 4];
      break;
    case 0x0002:
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x0F00) >> 8] &
        self->registers[(self->opcode & 0x00F0) >> 4];
      break;
    case 0x0003:
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x0F00) >> 8] ^
        self->registers[(self->opcode & 0x00F0) >> 4];
      break;
    case 0x0004:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] +
                             self->registers[(self->opcode & 0x00F0) >> 4] > 0xFF;
      self->registers[(self->opcode & 0x0F00) >> 8] +=
        self->registers[(self->opcode & 0x00F0) >> 4];
      break;
    case 0x0005:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] >
                             self->registers[(self->opcode & 0x00F0) >> 4];
      self->registers[(self->opcode & 0x0F00) >> 8] -=
        self->registers[(self->opcode & 0x00F0) >> 4];
      break;
    case 0x0006:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] & 0x01;
      self->registers[(self->opcode & 0x0F00) >> 8] >>= 1;
      break;
    case 0x0007:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] <
                             self->registers[(self->opcode & 0x00F0) >> 4];
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x00F0) >> 4] -
        self->registers[(self->opcode & 0x0F00) >> 8];
      break;
    case 0x000E:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] >> 7;
      self->registers[(self->opcode & 0x0F00) >> 8] <<= 1;
      break;
    }
    chip8_next_opcode(self);
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

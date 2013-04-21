#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "chip8.h"

#define BYTE3(blob) ((blob & 0x0F00) >> 8)
#define BYTE2(blob) ((blob & 0x00F0) >> 4)
#define BYTE1(blob) ((blob & 0x000F))

chip8_t * chip8_new(void) {
  chip8_t * self = (chip8_t *) malloc(sizeof(chip8_t));

  self->program_counter = 0x200;
  self->index_register  = 0;
  self->stack_pointer   = 0;
  self->sound_timer     = 0;
  self->delay_timer     = 0;
  self->opcode          = 0;

  memcpy(self->memory, chip8_hex_font, sizeof(chip8_hex_font));
  memset(self->screen, 0, sizeof(self->screen));
  memset(self->keys, 0, sizeof(self->keys));
  memset(self->stack, 0, sizeof(self->stack));

  srand(time(0));

  return self;
}

void chip8_tick(chip8_t * self) {
  self->opcode = self->memory[PC(self)] << 8 | self->memory[PC(self) + 1];

  switch (self->opcode & 0xF000) {
  case 0x0000:
    switch (self->opcode & 0x00FF) {
    case 0x00E0:
      memset(self->screen, 0, sizeof(self->screen));
      PC(self) += 2;
      break;
    case 0x00EE:
      PC(self) = self->stack[--SP(self)] + 2;
      break;
    default:
      chip8_no_such_opcode(self);
    }
    break;
  case 0x1000:
    PC(self) = self->opcode & 0x0FFF;
    break;
  case 0x2000:
    self->stack[SP(self)++] = PC(self);
    PC(self) = self->opcode & 0x0FFF;
    break;
  case 0x3000:
    (V(self)[BYTE3(self->opcode)]) == (self->opcode & 0x00FF)
      ? (PC(self) += 4)
      : (PC(self) += 2);
    break;
  case 0x4000:
    (V(self)[BYTE3(self->opcode)]) == (self->opcode & 0x00FF)
      ? (PC(self) += 2)
      : (PC(self) += 4);
    break;
  case 0x5000:
    (V(self)[BYTE3(self->opcode)] == V(self)[BYTE2(self->opcode)])
      ? (PC(self) += 4)
      : (PC(self) += 2);
    break;
  case 0x6000:
    V(self)[BYTE3(self->opcode)] = self->opcode & 0x00FF;
    PC(self) += 2;
    break;
  case 0x7000:
    V(self)[BYTE3(self->opcode)] += self->opcode & 0x00FF;
    PC(self) += 2;
    break;
  case 0x8000:
    switch (self->opcode & 0x000F) {
    case 0x0000:
      V(self)[BYTE3(self->opcode)] = V(self)[BYTE2(self->opcode)];
      PC(self) += 2;
      break;
    case 0x0001:
      V(self)[BYTE3(self->opcode)] |= V(self)[BYTE2(self->opcode)];
      PC(self) += 2;
      break;
    case 0x0002:
      V(self)[BYTE3(self->opcode)] &= V(self)[BYTE2(self->opcode)];
      PC(self) += 2;
      break;
    case 0x0003:
      V(self)[BYTE3(self->opcode)] ^= V(self)[BYTE2(self->opcode)];
      PC(self) += 2;
      break;
    case 0x0004:
      VF(self) = V(self)[BYTE3(self->opcode)] + V(self)[BYTE2(self->opcode)] > 0xFF;
      V(self)[BYTE3(self->opcode)] += V(self)[BYTE2(self->opcode)];
      PC(self) += 2;
      break;
    case 0x0005:
      VF(self) = V(self)[BYTE2(self->opcode)] <= V(self)[BYTE3(self->opcode)];
      V(self)[BYTE3(self->opcode)] -= V(self)[BYTE2(self->opcode)];
      PC(self) += 2;
      break;
    case 0x0006:
      VF(self) = V(self)[BYTE3(self->opcode)] & 1;
      V(self)[BYTE3(self->opcode)] >>= 1;
      PC(self) += 2;
      break;
    case 0x0007:
      VF(self) = V(self)[BYTE3(self->opcode)] <= V(self)[BYTE2(self->opcode)];
      V(self)[BYTE3(self->opcode)] = V(self)[BYTE2(self->opcode)] - V(self)[BYTE3(self->opcode)];
      PC(self) += 2;
      break;
    case 0x000E:
      VF(self) = V(self)[BYTE3(self->opcode)] >> 7;
      V(self)[BYTE3(self->opcode)] <<= 1;
      PC(self) += 2;
      break;
    default:
      chip8_no_such_opcode(self);
    }
    break;
  case 0x9000:
    (V(self)[BYTE3(self->opcode)] == V(self)[BYTE2(self->opcode)])
      ? (PC(self) += 2)
      : (PC(self) += 4);
    break;
  case 0xA000:
    I(self) = self->opcode & 0x0FFF;
    PC(self) += 2;
    break;
  case 0xB000:
    PC(self) = V0(self) + (self->opcode & 0x0FFF);
    break;
  case 0xC000:
    V(self)[BYTE3(self->opcode)] = rand() & (self->opcode & 0x00FF);
    PC(self) += 2;
    break;
  case 0xD000:
    {
      unsigned char i, j, row, pixel;
      unsigned char x_coord = V(self)[BYTE3(self->opcode)];
      unsigned char y_coord = V(self)[BYTE2(self->opcode)];
      unsigned char height  = BYTE1(self->opcode);
      unsigned char width   = 8;

      VF(self) = 0;
      for (i = 0; i < height; i++) {
        row = self->memory[I(self) + i];
        for (j = 0; j < width; j++) {
          if (!(pixel = row & (0x80 >> j))) {
            continue;
          }
          if ((32 * (x_coord + j) + (y_coord + i)) > (64 * 32)) {
            continue;
          }
          if (self->screen[32 * (x_coord + j) + (y_coord + i)]) {
            VF(self) = 1;
          }
          self->screen[32 * (x_coord + j) + (y_coord + i)] ^= 1;
        }
      }
    }
    PC(self) += 2;
    break;
  case 0xE000:
    switch (self->opcode & 0xF0FF) {
    case 0xE09E:
      self->keys[V(self)[BYTE3(self->opcode)]]
        ? (PC(self) += 4)
        : (PC(self) += 2);
      break;
    case 0xE0A1:
      self->keys[V(self)[BYTE3(self->opcode)]]
        ? (PC(self) += 2)
        : (PC(self) += 4);
      break;
    default:
      chip8_no_such_opcode(self);
    }
    break;
  case 0xF000:
    switch (self->opcode & 0xF0FF) {
    case 0xF007:
      V(self)[BYTE3(self->opcode)] = DT(self);
      PC(self) += 2;
      break;
    case 0xF00A:
      {
        unsigned char i;

        for (i = 0; i < 16; i++) {
          if (self->keys[i]) {
            V(self)[BYTE3(self->opcode)] = i;
            PC(self) += 2;
            break;
          }
        }
      }
      break;
    case 0xF015:
      DT(self) = V(self)[BYTE3(self->opcode)];
      PC(self) += 2;
      break;
    case 0xF018:
      ST(self) = V(self)[BYTE3(self->opcode)];
      PC(self) += 2;
      break;
    case 0xF01E:
      VF(self) = (I(self) + V(self)[BYTE3(self->opcode)]) > 0xFFF;
      I(self) += V(self)[BYTE3(self->opcode)];
      PC(self) += 2;
      break;
    case 0xF029:
      I(self) = V(self)[BYTE3(self->opcode)] * 5;
      PC(self) += 2;
      break;
    case 0xF033:
      self->memory[I(self)]     = (V(self)[BYTE3(self->opcode)] / 100);
      self->memory[I(self) + 1] = (V(self)[BYTE3(self->opcode)] / 10) % 10;
      self->memory[I(self) + 2] = (V(self)[BYTE3(self->opcode)] % 100) % 10;
      PC(self) += 2;
      break;
    case 0xF055:
      {
        unsigned char i;

        for (i = 0; i <= BYTE3(self->opcode); i++) {
          self->memory[I(self)++] = V(self)[i];
        }
      }
      PC(self) += 2;
      break;
    case 0xF065:
      {
        unsigned char i;

        for (i = 0; i <= BYTE3(self->opcode); i++) {
          V(self)[i] = self->memory[I(self)++];
        }
      }
      PC(self) += 2;
      break;
    default:
      chip8_no_such_opcode(self);
    }
    break;
  default:
    chip8_no_such_opcode(self);
  }

  if (DT(self)) {
    DT(self)--;
  }

  if (ST(self)) {
    ST(self)--;
  }
}

void chip8_reset(chip8_t * self) {
  self->program_counter = 0x200;
  self->index_register  = 0;
  self->stack_pointer   = 0;
  self->sound_timer     = 0;
  self->delay_timer     = 0;
  self->opcode          = 0;

  memset(self->screen, 0, sizeof(self->screen));
  memset(self->stack, 0, sizeof(self->stack));
}

int chip8_load_file(chip8_t * self, char * filename) {
  FILE        * file;
  unsigned long size, maximum_file_size = 4096 - 0x200;

  if (!(file = fopen(filename, "rb"))) {
    goto error;
  }

  fseek(file, 0, SEEK_END);
  if ((size = ftell(file)) > maximum_file_size) {
    goto error;
  }
  fseek(file, 0, SEEK_SET);

  if (fread(self->memory + 0x200, 1, size, file) != size) {
    goto error;
  }

  fclose(file);
  return 1;

error:
  if (file) {
    fclose(file);
  }
  perror("Error");
  return 0;
}

void chip8_no_such_opcode(chip8_t * self) {
  fprintf(stderr, "Unknown opcode: 0x%X\n", self->opcode);
}

void chip8_free(chip8_t * self) {
  free(self);
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "chip8.h"
#include "dsl.h"

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

void chip8_fetch_opcode(chip8_t * self) {
  self->opcode = self->memory[PC(self)] << 8 | self->memory[PC(self) + 1];
}

void chip8_next_opcode(chip8_t * self) {
  PC(self) += 2;
}

void chip8_skip_next_opcode(chip8_t * self) {
  PC(self) += 4;
}

void chip8_decode_opcode(chip8_t * self) {
  switch (self->opcode & 0xF000) {
  case 0x0000:
    switch (self->opcode & 0x00FF) {
    case 0x00E0:
      memset(self->screen, 0, sizeof(self->screen));
      chip8_next_opcode(self);
      break;
    case 0x00EE:
      PC(self) = self->stack[SP(self)--];
      break;
    default:
      chip8_no_such_opcode(self);
    }
    break;
  case 0x1000:
    PC(self) = self->opcode & 0x0FFF;
    break;
  case 0x2000:
    self->stack[++SP(self)] = PC(self);
    PC(self) = self->opcode & 0x0FFF;
    break;
  case 0x3000:
  case 0x4000:
    if (V(self)[_X__(self->opcode)] == (self->opcode & 0x00FF)) {
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
    (V(self)[_X__(self->opcode)] == V(self)[__X_(self->opcode)])
      ? chip8_skip_next_opcode(self)
      : chip8_next_opcode(self);
    break;
  case 0x6000:
    V(self)[_X__(self->opcode)] = self->opcode & 0x00FF;
    chip8_next_opcode(self);
    break;
  case 0x7000:
    V(self)[_X__(self->opcode)] += self->opcode & 0x00FF;
    chip8_next_opcode(self);
    break;
  case 0x8000:
    switch (self->opcode & 0x000F) {
    case 0x0000:
      V(self)[_X__(self->opcode)] = V(self)[__X_(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0x0001:
      V(self)[_X__(self->opcode)] = V(self)[_X__(self->opcode)] | V(self)[__X_(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0x0002:
      V(self)[_X__(self->opcode)] = V(self)[_X__(self->opcode)] & V(self)[__X_(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0x0003:
      V(self)[_X__(self->opcode)] = V(self)[_X__(self->opcode)] ^ V(self)[__X_(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0x0004:
      VF(self) = V(self)[_X__(self->opcode)] + V(self)[__X_(self->opcode)] > 0xFF;
      V(self)[_X__(self->opcode)] += V(self)[__X_(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0x0005:
      VF(self) = V(self)[_X__(self->opcode)] > V(self)[__X_(self->opcode)];
      V(self)[_X__(self->opcode)] -= V(self)[__X_(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0x0006:
      VF(self) = V(self)[_X__(self->opcode)] & 0x01;
      V(self)[_X__(self->opcode)] >>= 1;
      chip8_next_opcode(self);
      break;
    case 0x0007:
      VF(self) = V(self)[_X__(self->opcode)] < V(self)[__X_(self->opcode)];
      V(self)[_X__(self->opcode)] = V(self)[__X_(self->opcode)] - V(self)[_X__(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0x000E:
      VF(self) = V(self)[_X__(self->opcode)] >> 7;
      V(self)[_X__(self->opcode)] <<= 1;
      chip8_next_opcode(self);
      break;
    default:
      chip8_no_such_opcode(self);
    }
    break;
  case 0x9000:
    (V(self)[_X__(self->opcode)] == V(self)[__X_(self->opcode)])
      ? chip8_next_opcode(self)
      : chip8_skip_next_opcode(self);
    break;
  case 0xA000:
    I(self) = self->opcode & 0x0FFF;
    chip8_next_opcode(self);
    break;
  case 0xB000:
    PC(self) = V0(self) + (self->opcode & 0x0FFF);
    break;
  case 0xC000:
    V(self)[_X__(self->opcode)] = rand() & (self->opcode & 0x00FF);
    chip8_next_opcode(self);
    break;
  case 0xD000:
    {
      unsigned char i, j, row, pixel;
      unsigned char x_coord = V(self)[_X__(self->opcode)];
      unsigned char y_coord = V(self)[__X_(self->opcode)];
      unsigned char height  = ___X(self->opcode);
      unsigned char width   = 8;

      VF(self) = 0;
      for (i = 0; i < height; i++) {
        row = self->memory[I(self) + i];
        for (j = 0; j < width; j++) {
          if (!(pixel = row & (0x80 >> j))) {
            continue;
          }
          if (self->screen[32 * (x_coord + j) + (y_coord + i)]) {
            VF(self) = 1;
          }
          self->screen[32 * (x_coord + j) + (y_coord + i)] ^= 1;
        }
      }
    }
    chip8_next_opcode(self);
    break;
  case 0xE000:
    switch (self->opcode & 0xF0FF) {
    case 0xE09E:
      self->keys[V(self)[_X__(self->opcode)]]
        ? chip8_skip_next_opcode(self)
        : chip8_next_opcode(self);
      break;
    case 0xE0A1:
      self->keys[V(self)[_X__(self->opcode)]]
        ? chip8_next_opcode(self)
        : chip8_skip_next_opcode(self);
      break;
    default:
      chip8_next_opcode(self);
    }
    break;
  case 0xF000:
    switch (self->opcode & 0xF0FF) {
    case 0xF007:
      V(self)[_X__(self->opcode)] = DT(self);
      chip8_next_opcode(self);
      break;
    case 0xF00A:
      {
        unsigned char i;

        for (i = 0; i < 16; i++) {
          if (self->keys[i]) {
            V(self)[_X__(self->opcode)] = i;
            chip8_next_opcode(self);
            break;
          }
        }
      }
      break;
    case 0xF015:
      DT(self) = V(self)[_X__(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0xF018:
      ST(self) = V(self)[_X__(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0xF01E:
      VF(self) = I(self) + V(self)[_X__(self->opcode)] > 0xFFF;
      I(self) += V(self)[_X__(self->opcode)];
      chip8_next_opcode(self);
      break;
    case 0xF029:
      I(self) = V(self)[_X__(self->opcode)] * 5;
      chip8_next_opcode(self);
      break;
    case 0xF033:
      self->memory[I(self)]     = (V(self)[_X__(self->opcode)] / 100);
      self->memory[I(self) + 1] = (V(self)[_X__(self->opcode)] / 10) % 10;
      self->memory[I(self) + 2] = (V(self)[_X__(self->opcode)] % 100) % 10;
      chip8_next_opcode(self);
      break;
    case 0xF055:
      {
        unsigned char i;

        for (i = 0; i <= _X__(self->opcode); i++) {
          self->memory[I(self)++] = V(self)[i];
        }
      }
      chip8_next_opcode(self);
      break;
    case 0xF065:
      {
        unsigned char i;

        for (i = 0; i <= _X__(self->opcode); i++) {
          V(self)[i] = self->memory[I(self)++];
        }
      }
      chip8_next_opcode(self);
      break;
    default:
      chip8_no_such_opcode(self);
    }
    break;
  default:
    chip8_no_such_opcode(self);
  }

  DT(self)--;
  ST(self)--;
}

void chip8_tick(chip8_t * self) {
  chip8_fetch_opcode(self);
  chip8_decode_opcode(self);
}

int chip8_load_file(chip8_t * self, char * filename) {
  FILE *        file;
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

void chip8_render_to_terminal(chip8_t * self) {
  unsigned char i, j;

  for (j = 0; j < 32; j++) {
    for (i = 0; i < 64; i++) {
      printf(self->screen[32 * i + j] ? "█" : " ");
    }
    printf("\n");
  }
  printf("\033[32A");
}

void chip8_no_such_opcode(chip8_t * self) {
  fprintf(stderr, "Unknown opcode: 0x%X\n", self->opcode);
}

void chip8_free(chip8_t * self) {
  free(self);
}

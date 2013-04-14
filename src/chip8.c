#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "chip8.h"

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
  self->opcode = self->memory[self->program_counter] << 8 |
                 self->memory[self->program_counter + 1];
}

void chip8_next_opcode(chip8_t * self) {
  self->program_counter += 2;
}

void chip8_skip_next_opcode(chip8_t * self) {
  self->program_counter += 4;
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
      self->program_counter = self->stack[self->stack_pointer--];
      break;
    default:
      chip8_no_such_opcode(self);
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
    if (self->registers[(self->opcode & 0x0F00) >> 8] == (self->opcode & 0x00FF)) {
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
    (self->registers[(self->opcode & 0x0F00) >> 8] ==
     self->registers[(self->opcode & 0x00F0) >> 4])
      ? chip8_skip_next_opcode(self)
      : chip8_next_opcode(self);
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
    case 0x0000:
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x00F0) >> 4];
      chip8_next_opcode(self);
      break;
    case 0x0001:
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x0F00) >> 8] |
        self->registers[(self->opcode & 0x00F0) >> 4];
      chip8_next_opcode(self);
      break;
    case 0x0002:
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x0F00) >> 8] &
        self->registers[(self->opcode & 0x00F0) >> 4];
      chip8_next_opcode(self);
      break;
    case 0x0003:
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x0F00) >> 8] ^
        self->registers[(self->opcode & 0x00F0) >> 4];
      chip8_next_opcode(self);
      break;
    case 0x0004:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] +
                             self->registers[(self->opcode & 0x00F0) >> 4] > 0xFF;
      self->registers[(self->opcode & 0x0F00) >> 8] +=
        self->registers[(self->opcode & 0x00F0) >> 4];
      chip8_next_opcode(self);
      break;
    case 0x0005:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] >
                             self->registers[(self->opcode & 0x00F0) >> 4];
      self->registers[(self->opcode & 0x0F00) >> 8] -=
        self->registers[(self->opcode & 0x00F0) >> 4];
      chip8_next_opcode(self);
      break;
    case 0x0006:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] & 0x01;
      self->registers[(self->opcode & 0x0F00) >> 8] >>= 1;
      chip8_next_opcode(self);
      break;
    case 0x0007:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] <
                             self->registers[(self->opcode & 0x00F0) >> 4];
      self->registers[(self->opcode & 0x0F00) >> 8] =
        self->registers[(self->opcode & 0x00F0) >> 4] -
        self->registers[(self->opcode & 0x0F00) >> 8];
      chip8_next_opcode(self);
      break;
    case 0x000E:
      self->registers[0xF] = self->registers[(self->opcode & 0x0F00) >> 8] >> 7;
      self->registers[(self->opcode & 0x0F00) >> 8] <<= 1;
      chip8_next_opcode(self);
      break;
    default:
      chip8_no_such_opcode(self);
    }
    break;
  case 0x9000:
    (self->registers[(self->opcode & 0x0F00) >> 8] ==
     self->registers[(self->opcode & 0x00F0) >> 4])
      ? chip8_next_opcode(self)
      : chip8_skip_next_opcode(self);
    break;
  case 0xA000:
    self->index_register = self->opcode & 0x0FFF;
    chip8_next_opcode(self);
    break;
  case 0xB000:
    self->program_counter = self->registers[0] + (self->opcode & 0x0FFF);
    break;
  case 0xC000:
    self->registers[(self->opcode & 0x0F00) >> 8] = rand() & (self->opcode & 0x00FF);
    chip8_next_opcode(self);
    break;
  case 0xD000:
    {
      unsigned char i, j, row, pixel;
      unsigned char x_coord = self->registers[(self->opcode & 0x0F00) >> 8];
      unsigned char y_coord = self->registers[(self->opcode & 0x00F0) >> 4];
      unsigned char height  = self->opcode & 0x000F;
      unsigned char width   = 8;

      self->registers[0xF] = 0;
      for (i = 0; i < height; i++) {
        row = self->memory[self->index_register + i];
        for (j = 0; j < width; j++) {
          if (!(pixel = row & (0x80 >> j))) {
            continue;
          }
          if (self->screen[32 * (x_coord + j) + (y_coord + i)]) {
            self->registers[0xF] = 1;
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
      self->keys[self->registers[(self->opcode & 0x0F00) >> 8]]
        ? chip8_skip_next_opcode(self)
        : chip8_next_opcode(self);
      break;
    case 0xE0A1:
      self->keys[self->registers[(self->opcode & 0x0F00) >> 8]]
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
      self->registers[(self->opcode & 0x0F00) >> 8] = self->delay_timer;
      chip8_next_opcode(self);
      break;
    case 0xF00A:
      {
        unsigned char i;

        for (i = 0; i < 16; i++) {
          if (self->keys[i]) {
            self->registers[(self->opcode & 0x0F00) >> 8] = i;
            chip8_next_opcode(self);
            break;
          }
        }
      }
      break;
    case 0xF015:
      self->delay_timer = self->registers[(self->opcode & 0x0F00) >> 8];
      chip8_next_opcode(self);
      break;
    case 0xF018:
      self->sound_timer = self->registers[(self->opcode & 0x0F00) >> 8];
      chip8_next_opcode(self);
      break;
    case 0xF01E:
      self->registers[0xF] = self->index_register +
                             self->registers[(self->opcode & 0x0F00) >> 8] > 0xFFF;
      self->index_register += self->registers[(self->opcode & 0x0F00) >> 8];
      chip8_next_opcode(self);
      break;
    case 0xF029:
      self->index_register = self->registers[(self->opcode & 0x0F00) >> 8] * 5;
      chip8_next_opcode(self);
      break;
    case 0xF033:
      self->memory[self->index_register] =
        self->registers[(self->opcode & 0x0F00) >> 8] / 100;
      self->memory[self->index_register + 1] =
        (self->registers[(self->opcode & 0x0F00) >> 8] / 10) % 10;
      self->memory[self->index_register + 2] =
        (self->registers[(self->opcode & 0x0F00) >> 8] % 100) % 10;
      chip8_next_opcode(self);
      break;
    case 0xF055:
      {
        unsigned char i;

        for (i = 0; i <= (self->opcode & 0x0F00) >> 8; i++) {
          self->memory[self->index_register++] = self->registers[i];
        }
      }
      chip8_next_opcode(self);
      break;
    case 0xF065:
      {
        unsigned char i;

        for (i = 0; i <= (self->opcode & 0x0F00) >> 8; i++) {
          self->registers[i] = self->memory[self->index_register++];
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

  self->delay_timer--;
  self->sound_timer--;
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

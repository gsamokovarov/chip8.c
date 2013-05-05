#include <stdio.h>
#include "test.h"
#include "chip8.h"

TEST(chip8_new) {
  unsigned int i;
  chip8_t * chip8 = chip8_new();

  i = 0;
  while (i < sizeof(chip8->keys) / sizeof(chip8->keys[0])) {
    assert(chip8->keys[i++] == 0);
  }

  i = 0;
  while (i < sizeof(chip8->stack) / sizeof(chip8->stack[0])) {
    assert(chip8->stack[i++] == 0);
  }

  i = 0;
  while (i < sizeof(chip8->screen) / sizeof(chip8->screen[0])) {
    assert(chip8->screen[i++] == 0);
  }

  assert(chip8->program_counter == 0x200);

  chip8_free(chip8);
}

TEST(chip8_clear_screen) {
  unsigned int i;
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x00;
  chip8->memory[0x201] = 0xE0;

  chip8_tick(chip8);

  i = 0;
  while (i < sizeof(chip8->screen) / sizeof(chip8->screen[0])) {
    assert(chip8->screen[i++] == 0);
  }

  assert(chip8->program_counter == 0x202);

  chip8_free(chip8);
}

TEST(chip8_return) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x00;
  chip8->memory[0x201] = 0xEE;

  chip8->stack[1] = 0x200;
  chip8->stack_pointer = 2;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x202);
  assert(chip8->stack_pointer == 1);

  chip8_free(chip8);
}

TEST(chip8_instruction_jump) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x11;
  chip8->memory[0x201] = 0x23;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x123);

  chip8_free(chip8);
}

TEST(chip8_call) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x21;
  chip8->memory[0x201] = 0x23;

  chip8_tick(chip8);

  assert(chip8->stack[chip8->stack_pointer - 1] == 0x200);
  assert(chip8->program_counter == 0x123);

  chip8_free(chip8);
}

TEST(chip8_skip_next_if_vx_is_kk) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x31;
  chip8->memory[0x201] = 0x42;

  chip8->memory[0x202] = 0x00;
  chip8->memory[0x203] = 0xEE;

  chip8->memory[0x204] = 0x31;
  chip8->memory[0x205] = 0x43;

  chip8->registers[1] = 0x42;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x204);

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x206);

  chip8_free(chip8);
}

TEST(chip8_skip_next_if_vx_isnt_kk) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x41;
  chip8->memory[0x201] = 0x42;

  chip8->memory[0x202] = 0x41;
  chip8->memory[0x203] = 0x43;

  chip8->registers[1] = 0x42;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x202);

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x206);

  chip8_free(chip8);
}

TEST(chip8_skip_next_if_vx_is_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x51;
  chip8->memory[0x201] = 0x20;

  chip8->memory[0x202] = 0x41;
  chip8->memory[0x203] = 0x43;

  chip8->memory[0x204] = 0x51;
  chip8->memory[0x205] = 0x30;

  chip8->registers[1] = 0x42;
  chip8->registers[2] = 0x42;
  chip8->registers[3] = 0x41;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x204);

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x206);

  chip8_free(chip8);
}

TEST(chip8_skip_next_if_vx_isnt_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x91;
  chip8->memory[0x201] = 0x20;

  chip8->memory[0x202] = 0x41;
  chip8->memory[0x203] = 0x43;

  chip8->memory[0x204] = 0x91;
  chip8->memory[0x205] = 0x30;

  chip8->registers[1] = 0x42;
  chip8->registers[2] = 0x12;
  chip8->registers[3] = 0x42;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x204);

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x206);

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_kk) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x61;
  chip8->memory[0x201] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vx_plus_kk) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x61;
  chip8->memory[0x201] = 0x42;

  chip8->memory[0x202] = 0x71;
  chip8->memory[0x203] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_tick(chip8);

  assert(chip8->registers[1] == (0x42 + 0x42));

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x20;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vx_or_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x21;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (0x00 | 0x42));

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vx_and_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x22;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x00);

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vx_xor_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x23;

  chip8->registers[1] = 0x11;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (0x11 ^ 0x42));

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vx_plus_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x24;

  chip8->registers[1] = 0xFF;
  chip8->registers[2] = 0xFF;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0xFF + 0xFF));
  assert(chip8->registers[15] == 1);

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vx_sub_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x25;

  chip8->registers[1] = 0x42;
  chip8->registers[2] = 0xFF;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0x42 - 0xFF));
  assert(chip8->registers[15] == 0);

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vx_shm_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x26;

  chip8->registers[1] = 0x05;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0x05 >> 1));
  assert(chip8->registers[15] == 1);

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vy_sub_vx) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x27;

  chip8->registers[1] = 0x42;
  chip8->registers[2] = 0xFF;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0xFF - 0x42));
  assert(chip8->registers[15] == 1);

  chip8_free(chip8);
}

TEST(chip8_set_vx_to_vx_shl_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x2E;

  chip8->registers[1] = 0x81;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0x81 << 1));
  assert(chip8->registers[15] == 1);

  chip8_free(chip8);
}

TEST(chip8_instruction_jump_plus_v0) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0xB1;
  chip8->memory[0x201] = 0x23;

  chip8->registers[0] = 0x42;

  chip8_tick(chip8);

  assert(chip8->program_counter == (0x123 + 0x42));

  chip8_free(chip8);
}

TEST(chip8_draw_xor_mode) {
  chip8_t * chip8 = chip8_new();

  chip8->registers[0] = 0;

  chip8->memory[0x200] = 0xF0;
  chip8->memory[0x201] = 0x29;

  chip8->memory[0x202] = 0xD0;
  chip8->memory[0x203] = 0x05;

  chip8->memory[0x204] = 0x12;
  chip8->memory[0x205] = 0x02;

  chip8_tick(chip8);
  chip8_tick(chip8);

  assert(chip8->screen[0]      == 1);
  assert(chip8->registers[0xF] == 0);

  chip8_tick(chip8);
  chip8_tick(chip8);

  assert(chip8->screen[0]      == 0);
  assert(chip8->registers[0xF] == 1);

  chip8_free(chip8);
}

TEST(chip8_get_hex_sprite_location) {
  unsigned int i;
  chip8_t * chip8 = chip8_new();

  for (i = 0; i < 16; i++) {
    chip8->registers[i] = i;
  }

  for (i = 0; i < 16; i++) {
    chip8->memory[0x200 + (i * 2)]     = 0xF0 + i;
    chip8->memory[0x200 + (i * 2) + 1] = 0x29;
  }

  for (i = 0; i < 16; i++) {
    chip8_tick(chip8);

    assert(chip8->index_register == i * 5);
  }

  chip8_free(chip8);
}

TEST(chip8_binary_coded_decimal) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0xF0;
  chip8->memory[0x201] = 0x33;

  chip8->registers[0] = 255;

  chip8_tick(chip8);

  assert(chip8->memory[chip8->index_register++] == 0x02);
  assert(chip8->memory[chip8->index_register++] == 0x05);
  assert(chip8->memory[chip8->index_register]   == 0x05);

  chip8_free(chip8);
}

TEST(chip8_copy_memory_into_registers) {
  unsigned int i;
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0xA2;
  chip8->memory[0x201] = 0x04;
  chip8->memory[0x202] = 0xFF;
  chip8->memory[0x203] = 0x65;

  for (i = 0; i < 16; i++) {
    chip8->memory[0x204 + i] = i;
  }

  chip8_tick(chip8);

  assert(chip8->index_register == 0x204);

  chip8_tick(chip8);

  for (i = 0; i < 16; i++) {
    assert(chip8->registers[i] == i);
  }

  assert(chip8->index_register == (0x204 + 16));

  chip8_free(chip8);
}

TEST(chip8_copy_registers_into_memory) {
  unsigned int i;
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0xA2;
  chip8->memory[0x201] = 0x04;
  chip8->memory[0x202] = 0xFF;
  chip8->memory[0x203] = 0x55;

  for (i = 0; i < 16; i++) {
    chip8->registers[i] = i;
  }

  chip8_tick(chip8);

  assert(chip8->index_register == 0x204);

  chip8_tick(chip8);

  for (i = 0; i < 16; i++) {
    assert(chip8->memory[0x204 + i] == i);
  }

  assert(chip8->index_register == (0x204 + 16));

  chip8_free(chip8);
}

static test_t chip8_test_suite[] = {
  UNIT(chip8_new),
  UNIT(chip8_clear_screen),
  UNIT(chip8_return),
  UNIT(chip8_instruction_jump),
  UNIT(chip8_call),
  UNIT(chip8_skip_next_if_vx_is_kk),
  UNIT(chip8_skip_next_if_vx_isnt_kk),
  UNIT(chip8_skip_next_if_vx_is_vy),
  UNIT(chip8_skip_next_if_vx_isnt_vy),
  UNIT(chip8_set_vx_to_kk),
  UNIT(chip8_set_vx_to_vx_plus_kk),
  UNIT(chip8_set_vx_to_vy),
  UNIT(chip8_set_vx_to_vx_or_vy),
  UNIT(chip8_set_vx_to_vx_and_vy),
  UNIT(chip8_set_vx_to_vx_xor_vy),
  UNIT(chip8_set_vx_to_vx_plus_vy),
  UNIT(chip8_set_vx_to_vx_sub_vy),
  UNIT(chip8_set_vx_to_vx_shm_vy),
  UNIT(chip8_set_vx_to_vy_sub_vx),
  UNIT(chip8_set_vx_to_vx_shl_vy),
  UNIT(chip8_instruction_jump_plus_v0),
  UNIT(chip8_draw_xor_mode),
  UNIT(chip8_get_hex_sprite_location),
  UNIT(chip8_binary_coded_decimal),
  UNIT(chip8_copy_memory_into_registers),
  UNIT(chip8_copy_registers_into_memory)
};

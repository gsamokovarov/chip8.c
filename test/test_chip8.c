#include "test.h"
#include "chip8.h"

TEST(new) {
  unsigned int i;
  chip8_t * chip8 = chip8_new();

  i = 0;
  while (i < sizeof(chip8->keys)) {
    assert(chip8->keys[i++] == 0);
  }

  i = 0;
  while (i < sizeof(chip8->stack)) {
    assert(chip8->stack[i++] == 0);
  }

  i = 0;
  while (i < sizeof(chip8->screen)) {
    assert(chip8->screen[i++] == 0);
  }

  assert(chip8->program_counter == 0x200);

  chip8_free(chip8);
}

TEST(clear_screen) {
  unsigned int i;
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x00;
  chip8->memory[0x201] = 0xE0;

  chip8_tick(chip8);

  i = 0;
  while (i < sizeof(chip8->screen)) {
    assert(chip8->screen[i++] == 0);
  }

  assert(chip8->program_counter == 0x202);

  chip8_free(chip8);
}

TEST(return) {
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

TEST(instruction_jump) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x11;
  chip8->memory[0x201] = 0x23;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x123);

  chip8_free(chip8);
}

TEST(call) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x21;
  chip8->memory[0x201] = 0x23;

  chip8_tick(chip8);

  assert(chip8->stack[chip8->stack_pointer - 1] == 0x200);
  assert(chip8->program_counter == 0x123);

  chip8_free(chip8);
}

TEST(skip_next_if_vx_is_kk) {
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

TEST(skip_next_if_vx_isnt_kk) {
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

TEST(skip_next_if_vx_is_vy) {
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

TEST(skip_next_if_vx_isnt_vy) {
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

TEST(set_vx_to_kk) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x61;
  chip8->memory[0x201] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_free(chip8);
}

TEST(set_vx_to_vx_plus_kk) {
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

TEST(set_vx_to_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x20;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_free(chip8);
}

TEST(set_vx_to_vx_or_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x21;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (0x00 | 0x42));

  chip8_free(chip8);
}

TEST(set_vx_to_vx_and_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x22;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x00);

  chip8_free(chip8);
}

TEST(set_vx_to_vx_xor_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x23;

  chip8->registers[1] = 0x11;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (0x11 ^ 0x42));

  chip8_free(chip8);
}

TEST(set_vx_to_vx_plus_vy) {
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

TEST(set_vx_to_vx_sub_vy) {
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

TEST(set_vx_to_vx_shm_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x26;

  chip8->registers[1] = 0x05;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0x05 >> 1));
  assert(chip8->registers[15] == 1);

  chip8_free(chip8);
}

TEST(set_vx_to_vy_sub_vx) {
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

TEST(set_vx_to_vx_shl_vy) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x2E;

  chip8->registers[1] = 0x81;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0x81 << 1));
  assert(chip8->registers[15] == 1);

  chip8_free(chip8);
}

TEST(instruction_jump_plus_v0) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0xB1;
  chip8->memory[0x201] = 0x23;

  chip8->registers[0] = 0x42;

  chip8_tick(chip8);

  assert(chip8->program_counter == (0x123 + 0x42));

  chip8_free(chip8);
}

TEST(draw_xor_mode) {
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

TEST(get_hex_sprite_location) {
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

TEST(binary_coded_decimal) {
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

TEST(copy_memory_into_registers) {
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

TEST(copy_registers_into_memory) {
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
  TEST_REF(new),
  TEST_REF(clear_screen),
  TEST_REF(return),
  TEST_REF(instruction_jump),
  TEST_REF(call),
  TEST_REF(skip_next_if_vx_is_kk),
  TEST_REF(skip_next_if_vx_isnt_kk),
  TEST_REF(skip_next_if_vx_is_vy),
  TEST_REF(skip_next_if_vx_isnt_vy),
  TEST_REF(set_vx_to_kk),
  TEST_REF(set_vx_to_vx_plus_kk),
  TEST_REF(set_vx_to_vy),
  TEST_REF(set_vx_to_vx_or_vy),
  TEST_REF(set_vx_to_vx_and_vy),
  TEST_REF(set_vx_to_vx_xor_vy),
  TEST_REF(set_vx_to_vx_plus_vy),
  TEST_REF(set_vx_to_vx_sub_vy),
  TEST_REF(set_vx_to_vx_shm_vy),
  TEST_REF(set_vx_to_vy_sub_vx),
  TEST_REF(set_vx_to_vx_shl_vy),
  TEST_REF(instruction_jump_plus_v0),
  TEST_REF(draw_xor_mode),
  TEST_REF(get_hex_sprite_location),
  TEST_REF(binary_coded_decimal),
  TEST_REF(copy_memory_into_registers),
  TEST_REF(copy_registers_into_memory),
  0
};

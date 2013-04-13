#include <assert.h>
#include <stdio.h>
#include "../src/chip8.h"

void test_new(void) {
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

void test_clear_screen(void) {
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

void test_return(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x00;
  chip8->memory[0x201] = 0xEE;

  chip8->stack[2] = 0x202;
  chip8->stack_pointer = 2;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x202);
  assert(chip8->stack_pointer == 1);

  chip8_free(chip8);
}

void test_instruction_jump(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x11;
  chip8->memory[0x201] = 0x23;

  chip8_tick(chip8);

  assert(chip8->program_counter == 0x123);

  chip8_free(chip8);
}

void test_call(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x21;
  chip8->memory[0x201] = 0x23;

  chip8_tick(chip8);

  assert(chip8->stack[chip8->stack_pointer] == 0x200);
  assert(chip8->program_counter == 0x123);

  chip8_free(chip8);
}

void test_skip_next_if_vx_is_kk(void) {
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

void test_skip_next_if_vx_isnt_kk(void) {
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

void test_skip_next_if_vx_is_vy(void) {
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

void test_skip_next_if_vx_isnt_vy(void) {
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

void test_set_vx_to_kk(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x61;
  chip8->memory[0x201] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_free(chip8);
}

void test_set_vx_to_vx_plus_kk(void) {
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

void test_set_vx_to_vy(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x20;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_free(chip8);
}

void test_set_vx_to_vx_or_vy(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x21;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (0x00 | 0x42));

  chip8_free(chip8);
}

void test_set_vx_to_vx_and_vy(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x22;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == 0x00);

  chip8_free(chip8);
}

void test_set_vx_to_vx_xor_vy(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x23;

  chip8->registers[1] = 0x11;
  chip8->registers[2] = 0x42;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (0x11 ^ 0x42));

  chip8_free(chip8);
}

void test_set_vx_to_vx_plus_vy(void) {
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

void test_set_vx_to_vx_sub_vy(void) {
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

void test_set_vx_to_vx_shm_vy(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x26;

  chip8->registers[1] = 0x05;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0x05 >> 1));
  assert(chip8->registers[15] == 1);

  chip8_free(chip8);
}

void test_set_vx_to_vy_sub_vx(void) {
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

void test_set_vx_to_vx_shl_vy(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x2E;

  chip8->registers[1] = 0x81;

  chip8_tick(chip8);

  assert(chip8->registers[1] == (unsigned char) (0x81 << 1));
  assert(chip8->registers[15] == 1);

  chip8_free(chip8);
}

void test_instruction_jump_plus_v0(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0xB1;
  chip8->memory[0x201] = 0x23;

  chip8->registers[0] = 0x42;

  chip8_tick(chip8);

  assert(chip8->program_counter == (0x123 + 0x42));

  chip8_free(chip8);
}

void test_get_hex_sprite_location(void) {
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

void test_binary_coded_decimal(void) {
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

void test_copy_memory_into_registers(void) {
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

void test_copy_registers_into_memory(void) {
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

int main(void) {
  test_new();
  test_clear_screen();
  test_return();
  test_instruction_jump();
  test_call();
  test_skip_next_if_vx_is_kk();
  test_skip_next_if_vx_isnt_kk();
  test_skip_next_if_vx_is_vy();
  test_skip_next_if_vx_isnt_vy();
  test_set_vx_to_kk();
  test_set_vx_to_vx_plus_kk();
  test_set_vx_to_vy();
  test_set_vx_to_vx_or_vy();
  test_set_vx_to_vx_and_vy();
  test_set_vx_to_vx_xor_vy();
  test_set_vx_to_vx_plus_vy();
  test_set_vx_to_vx_sub_vy();
  test_set_vx_to_vx_shm_vy();
  test_set_vx_to_vy_sub_vx();
  test_set_vx_to_vx_shl_vy();
  test_instruction_jump_plus_v0();
  test_get_hex_sprite_location();
  test_binary_coded_decimal();
  test_copy_memory_into_registers();
  test_copy_registers_into_memory();

  return 0;
}

#include <assert.h>
#include "../src/chip8.c"

void test_clear_screen(void) {
  int i;
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x00;
  chip8->memory[0x201] = 0xE0;

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  i = 0;
  while (i < 64 * 32) {
    assert(chip8->memory[i++] == 0);
  }

  chip8_free(chip8);
}

void test_instruction_jump(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x11;
  chip8->memory[0x201] = 0x23;

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->program_counter == 0x123);

  chip8_free(chip8);
}

void test_call(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x21;
  chip8->memory[0x201] = 0x23;

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

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

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->program_counter == 0x204);

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

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

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->program_counter == 0x202);

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

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

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->program_counter == 0x204);

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->program_counter == 0x206);

  chip8_free(chip8);
}

void test_set_vx_to_kk(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x61;
  chip8->memory[0x201] = 0x42;

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_free(chip8);
}

void test_set_vx_to_vx_plus_kk(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x61;
  chip8->memory[0x201] = 0x42;

  chip8->memory[0x202] = 0x71;
  chip8->memory[0x203] = 0x42;

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->registers[1] == (0x42 + 0x42));

  chip8_free(chip8);
}

void test_set_vx_to_vy(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x20;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->registers[1] == 0x42);

  chip8_free(chip8);
}

void test_set_vx_to_vx_xor_vy(void) {
  chip8_t * chip8 = chip8_new();

  chip8->memory[0x200] = 0x81;
  chip8->memory[0x201] = 0x21;

  chip8->registers[1] = 0x00;
  chip8->registers[2] = 0x42;

  chip8_fetch_current_opcode(chip8);
  chip8_decode_current_opcode(chip8);

  assert(chip8->registers[1] == (0x00 | 0x42));

  chip8_free(chip8);
}

int main(int argc, char ** argv) {
  test_clear_screen();
  test_instruction_jump();
  test_call();
  test_skip_next_if_vx_is_kk();
  test_skip_next_if_vx_isnt_kk();
  test_skip_next_if_vx_is_vy();
  test_set_vx_to_kk();
  test_set_vx_to_vx_plus_kk();
  test_set_vx_to_vy();
  test_set_vx_to_vx_xor_vy();

  return 0;
}

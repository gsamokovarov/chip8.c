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

int main(int argc, char ** argv) {
  test_clear_screen();
  test_instruction_jump();
  test_call();

  return 0;
}

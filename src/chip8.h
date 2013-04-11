#ifndef CHIP8_H
#define CHIP8_H

struct chip8 {
  unsigned short opcode;
  unsigned char  memory[4096];
  unsigned char  registers[16];
  unsigned short index_register;
  unsigned short program_counter;
  unsigned char  screen[64 * 32];
  unsigned char  delay_timer;
  unsigned char  sound_timer;
  unsigned short stack[16];
  unsigned short stack_pointer;
  unsigned char  keys[16];
};

typedef struct chip8 chip8_t;

unsigned char chip8_hex_font[80] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
  0x20, 0x60, 0x20, 0x20, 0xF0, /* 1 */
  0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
  0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
  0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
  0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
  0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
  0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
  0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
  0xF0, 0x90, 0xF0, 0x10, 0x10, /* 9 */
  0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
  0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
  0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
  0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
  0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
  0xF0, 0x80, 0xF0, 0x80, 0x80, /* F */
};

chip8_t * chip8_new(void);
void chip8_fetch_opcode(chip8_t *);
void chip8_decode_opcode(chip8_t *);
void chip8_tick(chip8_t *);
void chip8_next_opcode(chip8_t *);
void chip8_skip_next_opcode(chip8_t *);
int  chip8_load_file(chip8_t *, char *);
void chip8_no_such_opcode(chip8_t *);
void chip8_free(chip8_t *);

#endif

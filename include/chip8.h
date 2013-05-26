#ifndef _CHIP8_H
#define _CHIP8_H

#include <stdint.h>
#include <pthread.h>

#define BYTE3(blob) ((blob & 0x0F00) >> 8)
#define BYTE2(blob) ((blob & 0x00F0) >> 4)
#define BYTE1(blob) ((blob & 0x000F))

#define PC(chip8) chip8->program_counter
#define V(chip8)  chip8->registers
#define V0(chip8) chip8->registers[0]
#define VF(chip8) chip8->registers[0xF]
#define I(chip8)  chip8->index_register
#define DT(chip8) chip8->delay_timer->value
#define ST(chip8) chip8->sound_timer->value
#define SP(chip8) chip8->stack_pointer

typedef struct chip8_timer {
  uint8_t   value;
  int       active;
  int       done;
  pthread_t thread;
} chip8_timer_t;

chip8_timer_t * chip8_timer_new(void);
void chip8_timer_reset(chip8_timer_t *);
void * chip8_timer_loop(chip8_timer_t *);
void chip8_timer_free(chip8_timer_t *);

typedef struct chip8 {
  chip8_timer_t * delay_timer;
  chip8_timer_t * sound_timer;
  uint16_t opcode;
  uint8_t  memory[4096];
  uint8_t  registers[16];
  uint16_t index_register;
  uint16_t program_counter;
  uint8_t  screen[64 * 32];
  uint16_t stack[16];
  uint16_t stack_pointer;
  uint8_t  keys[16];
} chip8_t;

static const uint8_t chip8_hex_font[] = {
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
  0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

chip8_t * chip8_new(void);
void chip8_tick(chip8_t *);
void chip8_reset(chip8_t *);
int  chip8_load_rom(chip8_t *, char *);
void chip8_no_such_opcode(chip8_t *);
void chip8_free(chip8_t *);

#endif

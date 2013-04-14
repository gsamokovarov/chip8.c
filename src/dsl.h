#define PC(chip8)  chip8->program_counter
#define V(chip8)   chip8->registers
#define V0(chip8)  chip8->registers[0]
#define VF(chip8)  chip8->registers[0xF]
#define I(chip8)   chip8->index_register
#define DT(chip8)  chip8->delay_timer
#define ST(chip8)  chip8->sound_timer
#define SP(chip8)  chip8->stack_pointer

#define X___(blob) ((blob & 0xF000) >> 16)
#define _X__(blob) ((blob & 0x0F00) >> 8)
#define __X_(blob) ((blob & 0x00F0) >> 4)
#define ___X(blob) ((blob & 0x000F))

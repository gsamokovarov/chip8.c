#define PC(chip8) chip8->program_counter
#define V(chip8)  chip8->registers
#define V0(chip8) chip8->registers[0]
#define VF(chip8) chip8->registers[0xF]
#define I(chip8)  chip8->index_register
#define DT(chip8) chip8->delay_timer
#define ST(chip8) chip8->sound_timer
#define SP(chip8) chip8->stack_pointer

#define BYTE4(blob) ((blob & 0xF000) >> 16)
#define BYTE3(blob) ((blob & 0x0F00) >> 8)
#define BYTE2(blob) ((blob & 0x00F0) >> 4)
#define BYTE1(blob) ((blob & 0x000F))

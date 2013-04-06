struct chip8 {
  unsigned short opcode;
  unsigned char  memory[4096];
  unsigned char  general_purpose_registers[16];
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

chip8_t * chip8_new(void);
void chip8_free(chip8_t *);

#ifndef _TERMINAL_IO_H
#define _TERMINAL_IO_H

typedef struct io io_t;
typedef struct chip8 chip8_t;

io_t * terminal_io_new(void);
void terminal_io_setup(io_t *);
void terminal_io_render(io_t *, chip8_t *);
void terminal_io_beep(io_t *, chip8_t *);
void terminal_io_delay(io_t *, chip8_t *);
int  terminal_io_listen(io_t *, chip8_t *);
void terminal_io_teardown(io_t *);

#endif

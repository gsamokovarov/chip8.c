#ifndef TERMINALIO_H
#define TERMINALIO_H

typedef struct io io_t;
typedef struct chip8 chip8_t;

io_t * terminal_io_new(void);
void terminal_io_render(io_t *, chip8_t *);
void terminal_io_listen(io_t *, chip8_t *);

#endif

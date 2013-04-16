#ifndef TERMINALIO_H
#define TERMINALIO_H

typedef struct io io_t;
typedef struct chip8 chip8_t;

io_t * terminalio_new(void);
void terminalio_render(io_t *, chip8_t *);
void terminalio_listen(io_t *, chip8_t *);

#endif

#ifndef SDL_IO_H
#define SDL_IO_H

typedef struct io io_t;
typedef struct chip8 chip8_t;

io_t * sdl_io_new(void);
void sdl_io_setup(io_t *);
void sdl_io_render(io_t *, chip8_t *);
void sdl_io_listen(io_t *, chip8_t *);
void sdl_io_teardown(io_t *);

#endif

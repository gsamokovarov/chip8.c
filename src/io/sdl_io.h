#ifndef _SDL_IO_H
#define _SDL_IO_H

#define SDL_IO_CUSTOM(io) ((sdl_io_custom_t *) io->custom)

typedef struct io io_t;
typedef struct chip8 chip8_t;
typedef struct SDL_Surface SDL_Surface;

typedef struct sdl_io_custom {
  SDL_Surface * surface;
} sdl_io_custom_t;

io_t * sdl_io_new(void);
void sdl_io_setup(io_t *);
void sdl_io_render(io_t *, chip8_t *);
int  sdl_io_listen(io_t *, chip8_t *);
void sdl_io_teardown(io_t *);

#endif

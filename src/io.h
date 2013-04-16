#ifndef IO_H
#define IO_H

#define UNUSED(x) (void) x

typedef struct chip8 chip8_t;
typedef struct io io_t;

typedef void (io_render_t)(io_t *, chip8_t *);
typedef void (io_listen_t)(io_t *, chip8_t *);

typedef struct io {
  io_render_t * render;
  io_listen_t * listen;
} io_t;

void io_free(io_t *);

#endif

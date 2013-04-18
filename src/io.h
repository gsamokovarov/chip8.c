#ifndef _IO_H
#define _IO_H

#define UNUSED(x) (void) x

typedef struct chip8 chip8_t;
typedef struct io io_t;

typedef void (* io_setup_t)(io_t *);
typedef void (* io_render_t)(io_t *, chip8_t *);
typedef void (* io_listen_t)(io_t *, chip8_t *);
typedef void (* io_teardown_t)(io_t *);

typedef struct io {
  io_setup_t    setup;
  io_render_t   render;
  io_listen_t   listen;
  io_teardown_t teardown;
} io_t;

void io_setup(io_t *);
void io_render(io_t *, chip8_t *);
void io_listen(io_t *, chip8_t *);
void io_teardown(io_t *);
void io_free(io_t *);

#endif

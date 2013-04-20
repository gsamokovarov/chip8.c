#ifndef _IO_H
#define _IO_H

#define UNUSED(x) (void) x

typedef struct chip8 chip8_t;
typedef struct io io_t;

typedef void (* io_setup_t)(io_t *);
typedef void (* io_render_t)(io_t *, chip8_t *);
typedef void (* io_delay_t)(io_t *, chip8_t *);
typedef int  (* io_listen_t)(io_t *, chip8_t *);
typedef void (* io_teardown_t)(io_t *);
typedef void * io_custom_t;

typedef struct io {
  io_setup_t    setup;
  io_render_t   render;
  io_delay_t    delay;
  io_listen_t   listen;
  io_teardown_t teardown;
  io_custom_t   custom;
} io_t;

inline void io_setup(io_t *);
inline void io_render(io_t *, chip8_t *);
inline void io_delay(io_t *, chip8_t *);
inline int  io_listen(io_t *, chip8_t *);
inline void io_teardown(io_t *);
inline void io_free(io_t *);

#endif

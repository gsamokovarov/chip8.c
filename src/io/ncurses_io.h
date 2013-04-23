#ifndef _NCURSES_IO_H
#define _NCURSES_IO_H

typedef struct io io_t;
typedef struct chip8 chip8_t;

io_t * ncurses_io_new(void);
void ncurses_io_setup(io_t *);
void ncurses_io_render(io_t *, chip8_t *);
void ncurses_io_beep(io_t *, chip8_t *);
void ncurses_io_delay(io_t *, chip8_t *);
int  ncurses_io_listen(io_t *, chip8_t *);
void ncurses_io_teardown(io_t *);

#endif

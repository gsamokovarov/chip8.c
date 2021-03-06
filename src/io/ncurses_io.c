#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "io.h"
#include "chip8.h"
#include "io/ncurses_io.h"

io_t * ncurses_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  if (self) {
    self->name     = "ncurses";
    self->setup    = &ncurses_io_setup;
    self->render   = &ncurses_io_render;
    self->beep     = &ncurses_io_beep;
    self->listen   = &ncurses_io_listen;
    self->teardown = &ncurses_io_teardown;
    self->delay    = &ncurses_io_delay;
    self->custom   = 0;
  }

  return self;
}

void ncurses_io_setup(io_t * self) {
  UNUSED(self);

  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, 1);
  curs_set(0);
}

void ncurses_io_render(io_t * self, chip8_t * chip8) {
  UNUSED(self);

  unsigned char i, j;

  for (j = 0; j < 32; j++) {
    move(j, 0);
    for (i = 0; i < 64; i++) {
      addch(chip8->screen[32 * i + j] ? ' ' | A_REVERSE : ' ');
    }
  }
  refresh();
}

void ncurses_io_beep(io_t * self, chip8_t * chip8) {
  UNUSED(self);

  if (ST(chip8) == 1) {
    beep();
  }
}

int ncurses_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  return 1;
}

void ncurses_io_delay(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  usleep(1667);
}

void ncurses_io_teardown(io_t * self) {
  UNUSED(self);

  endwin();
}

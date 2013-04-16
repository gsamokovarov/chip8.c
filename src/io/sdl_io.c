#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "../io.h"
#include "../chip8.h"
#include "sdl_io.h"

SDL_Surface * surface;

io_t * sdl_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->render = &sdl_io_render;
  self->listen = &sdl_io_listen;

  SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO);

  surface = SDL_SetVideoMode(64 * 8, 32 * 8, 32, SDL_HWSURFACE);

  return self;
}

void sdl_io_render(io_t * self, chip8_t * chip8) {
  UNUSED(self);

  unsigned char i, j;
  Uint32 black, white;
  SDL_Rect rect;

  rect.w = 8, rect.h = 8;

  black = SDL_MapRGB(surface->format, 0, 0, 0);
  white = SDL_MapRGB(surface->format, 255, 255, 255);

  for (j = 0; j < 32; j++) {
    for (i = 0; i < 64; i++) {
      rect.x = (i * 8), rect.y = (j * 8);
      SDL_FillRect(surface, &rect, chip8->screen[32 * i + j] ? white : black);
    }
  }

  SDL_Flip(surface);
}

void sdl_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

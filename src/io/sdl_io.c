#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "../io.h"
#include "../chip8.h"
#include "sdl_io.h"

SDL_Surface * surface;

io_t * sdl_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->setup = &sdl_io_setup;
  self->render = &sdl_io_render;
  self->listen = &sdl_io_listen;
  self->teardown = &sdl_io_teardown;

  return self;
}

void sdl_io_setup(io_t * self) {
  UNUSED(self);

  SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO);

  surface = SDL_SetVideoMode(64 * 8, 32 * 8, 32, SDL_HWSURFACE);
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

  SDL_Event event;

  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_KP_PERIOD:
      chip8->keys[0] = 1;
      break;
    case SDLK_KP7:
      chip8->keys[1] = 1;
      break;
    case SDLK_KP8:
      chip8->keys[2] = 1;
      break;
    case SDLK_KP9:
      chip8->keys[3] = 1;
      break;
    case SDLK_KP4:
      chip8->keys[4] = 1;
      break;
    case SDLK_KP5:
      chip8->keys[5] = 1;
      break;
    case SDLK_KP6:
      chip8->keys[6] = 1;
      break;
    case SDLK_KP1:
      chip8->keys[7] = 1;
      break;
    case SDLK_KP2:
      chip8->keys[8] = 1;
      break;
    case SDLK_KP3:
      chip8->keys[9] = 1;
      break;
    case SDLK_KP0:
      chip8->keys[10] = 1;
      break;
    case SDLK_KP_ENTER:
      chip8->keys[11] = 1;
      break;
    case SDLK_KP_DIVIDE:
      chip8->keys[12] = 1;
      break;
    case SDLK_KP_MULTIPLY:
      chip8->keys[13] = 1;
      break;
    case SDLK_KP_MINUS:
      chip8->keys[14] = 1;
      break;
    case SDLK_KP_PLUS:
      chip8->keys[15] = 1;
      break;
    default:
      break;
    }
    break;
  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
    case SDLK_KP_PERIOD:
      chip8->keys[0] = 0;
      break;
    case SDLK_KP7:
      chip8->keys[1] = 0;
      break;
    case SDLK_KP8:
      chip8->keys[2] = 0;
      break;
    case SDLK_KP9:
      chip8->keys[3] = 0;
      break;
    case SDLK_KP4:
      chip8->keys[4] = 0;
      break;
    case SDLK_KP5:
      chip8->keys[5] = 0;
      break;
    case SDLK_KP6:
      chip8->keys[6] = 0;
      break;
    case SDLK_KP1:
      chip8->keys[7] = 0;
      break;
    case SDLK_KP2:
      chip8->keys[8] = 0;
      break;
    case SDLK_KP3:
      chip8->keys[9] = 0;
      break;
    case SDLK_KP0:
      chip8->keys[10] = 0;
      break;
    case SDLK_KP_ENTER:
      chip8->keys[11] = 0;
      break;
    case SDLK_KP_DIVIDE:
      chip8->keys[12] = 0;
      break;
    case SDLK_KP_MULTIPLY:
      chip8->keys[13] = 0;
      break;
    case SDLK_KP_MINUS:
      chip8->keys[14] = 0;
      break;
    case SDLK_KP_PLUS:
      chip8->keys[15] = 0;
      break;
    default:
      break;
    }
    break;
  }
}

void sdl_io_teardown(io_t * self) {
  UNUSED(self);

  SDL_Quit();
}

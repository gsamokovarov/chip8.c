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
      chip8->keys[0x0] = 1;
      break;
    case SDLK_KP7:
      chip8->keys[0x1] = 1;
      break;
    case SDLK_KP8:
      chip8->keys[0x2] = 1;
      break;
    case SDLK_KP9:
      chip8->keys[0x3] = 1;
      break;
    case SDLK_KP4:
      chip8->keys[0x4] = 1;
      break;
    case SDLK_KP5:
      chip8->keys[0x5] = 1;
      break;
    case SDLK_KP6:
      chip8->keys[0x6] = 1;
      break;
    case SDLK_KP1:
      chip8->keys[0x7] = 1;
      break;
    case SDLK_KP2:
      chip8->keys[0x8] = 1;
      break;
    case SDLK_KP3:
      chip8->keys[0x9] = 1;
      break;
    case SDLK_KP0:
      chip8->keys[0xA] = 1;
      break;
    case SDLK_KP_ENTER:
      chip8->keys[0xB] = 1;
      break;
    case SDLK_KP_DIVIDE:
      chip8->keys[0xC] = 1;
      break;
    case SDLK_KP_MULTIPLY:
      chip8->keys[0xD] = 1;
      break;
    case SDLK_KP_MINUS:
      chip8->keys[0xE] = 1;
      break;
    case SDLK_KP_PLUS:
      chip8->keys[0xF] = 1;
      break;
    default:
#if CHIP8_DEBUG
      fprintf(stderr, "Unknown key: 0x%X\n", event.key.keysym.sym);
#endif
      break;
    }
    break;
  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
    case SDLK_KP_PERIOD:
      chip8->keys[0x0] = 0;
      break;
    case SDLK_KP7:
      chip8->keys[0x1] = 0;
      break;
    case SDLK_KP8:
      chip8->keys[0x2] = 0;
      break;
    case SDLK_KP9:
      chip8->keys[0x3] = 0;
      break;
    case SDLK_KP4:
      chip8->keys[0x4] = 0;
      break;
    case SDLK_KP5:
      chip8->keys[0x5] = 0;
      break;
    case SDLK_KP6:
      chip8->keys[0x6] = 0;
      break;
    case SDLK_KP1:
      chip8->keys[0x7] = 0;
      break;
    case SDLK_KP2:
      chip8->keys[0x8] = 0;
      break;
    case SDLK_KP3:
      chip8->keys[0x9] = 0;
      break;
    case SDLK_KP0:
      chip8->keys[0xA] = 0;
      break;
    case SDLK_KP_ENTER:
      chip8->keys[0xB] = 0;
      break;
    case SDLK_KP_DIVIDE:
      chip8->keys[0xC] = 0;
      break;
    case SDLK_KP_MULTIPLY:
      chip8->keys[0xD] = 0;
      break;
    case SDLK_KP_MINUS:
      chip8->keys[0xE] = 0;
      break;
    case SDLK_KP_PLUS:
      chip8->keys[0xF] = 0;
      break;
    default:
#if CHIP8_DEBUG
      fprintf(stderr, "Unknown key: 0x%X\n", event.key.keysym.sym);
#endif
      break;
    }
    break;
  }
}

void sdl_io_teardown(io_t * self) {
  UNUSED(self);

  SDL_Quit();
}

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "io.h"
#include "chip8.h"
#include "io/sdl_io.h"

sdl_io_custom_t * sdl_io_custom_new(void) {
  sdl_io_custom_t * self = (sdl_io_custom_t *) malloc(sizeof(sdl_io_custom_t));

  self->audio_spec = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));

  self->audio_spec->freq     = 44100;
  self->audio_spec->format   = AUDIO_S16SYS;
  self->audio_spec->channels = 1;
  self->audio_spec->samples  = 512;
  self->audio_spec->callback = 0;
  self->audio_spec->userdata = 0;

  self->surface = SDL_SetVideoMode(64 * 12, 32 * 12, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

  return self;
}

void sdl_io_custom_free(sdl_io_custom_t * self) {
  if (self->audio_spec) free(self->audio_spec);
  SDL_FreeSurface(self->surface);
}

io_t * sdl_io_new(void) {
  io_t * self = (io_t *) malloc(sizeof(io_t));

  self->name     = "sdl";
  self->setup    = &sdl_io_setup;
  self->render   = &sdl_io_render;
  self->beep     = &sdl_io_beep;
  self->listen   = &sdl_io_listen;
  self->delay    = &sdl_io_delay;
  self->teardown = &sdl_io_teardown;
  self->custom   = sdl_io_custom_new();

  return self;
}

void sdl_io_setup(io_t * self) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_WM_SetCaption("CHIP-8", 0);
}

void sdl_io_render(io_t * self, chip8_t * chip8) {
  Uint8 i, j;
  Uint32 black, white;
  SDL_Rect rect;

  rect.w = 12;
  rect.h = 12;

  black = SDL_MapRGB(SDL_IO_CUSTOM(self)->surface->format, 0, 0, 0);
  white = SDL_MapRGB(SDL_IO_CUSTOM(self)->surface->format, 255, 255, 255);

  for (j = 0; j < 32; j++) {
    for (i = 0; i < 64; i++) {
      rect.x = (i * 12);
      rect.y = (j * 12);

      SDL_FillRect(SDL_IO_CUSTOM(self)->surface, &rect, chip8->screen[32 * i + j] ? white : black);
    }
  }

  SDL_Flip(SDL_IO_CUSTOM(self)->surface);
}

void sdl_io_beep(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);
}

int sdl_io_listen(io_t * self, chip8_t * chip8) {
  UNUSED(self);

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return 0;
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
      case SDLK_ESCAPE:
        return 0;
      case SDLK_BACKSPACE:
        chip8_reset(chip8);
        break;
      default:
#if CHIP8_DEBUG
        if (event.key.keysym.sym) {
          fprintf(stderr, "Unknown key: 0x%X\n", event.key.keysym.sym);
        }
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
        break;
      }
      break;
    }
  }

  return 1;
}

void sdl_io_delay(io_t * self, chip8_t * chip8) {
  UNUSED(self);
  UNUSED(chip8);

  SDL_Delay(1);
}

void sdl_io_teardown(io_t * self) {
  if (self->custom) {
    sdl_io_custom_free(SDL_IO_CUSTOM(self));
    self->custom = 0;
  }
  SDL_Quit();
}

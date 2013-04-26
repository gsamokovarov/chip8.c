project(chip8.c)

# The minimum cmake version we need is 2.6. I am quite new to cmake so this may
# not be true.

cmake_minimum_required(VERSION 2.6)

# Package requirements. The most stable IO at the moment is the SDL one, so we
# have a hard dependency on it,

find_package(SDL 1.2.0 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})

# We softly depend on ncurses too.

find_package(CURSES)
if (CURSES_FOUND AND CURSES_USE_NCURSES)
  include_directories(${CURSES_INCLUDE_DIRS})
endif()

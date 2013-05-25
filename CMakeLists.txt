project(chip8.c)

# The minimum cmake version we need is 2.6. I am quite new to cmake so this may
# not be true.
cmake_minimum_required(VERSION 2.6)

# Let's start with the package requirements

# We really need some form of threads.
find_package(Threads REQUIRED)

# The most stable IO at the moment is the SDL one, so we have a hard dependency
# on it.
find_package(SDL 1.2.0 REQUIRED)
find_package(SDL_mixer 1.2.0 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS} ${SDLMIXER_INCLUDE_DIRS})
link_directories(${SDL_LIBRARY_DIRS} ${SDLMIXER_LIBRARY_DIRS})

# We softly depend on ncurses too.
find_package(Curses)
if (CURSES_FOUND)
  include_directories(${CURSES_INCLUDE_DIRS})
  link_directories(${CURSES_LIBRARY_DIRS})
endif()

# Now that we are done with the dependencies, lets add the essential stuff.

# Include our own headers, which live in the include directory.
include_directories(include)

# Use a generated config.h. This allow us to bubble options from the CMake
# configuration to the C compilator.
configure_file("include/config.h.in" "${CMAKE_SOURCE_DIR}/include/config.h")

# Add all of the sources under src.
file(GLOB_RECURSE CHIP8_SOURCES src/*.c)

# Define our one and only executable.
add_executable(chip8 ${CHIP8_SOURCES})

target_link_libraries(chip8 ${SDL_LIBRARY} ${SDLMIXER_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
if (CURSES_FOUND)
  target_link_libraries(chip8 ${CURSES_LIBRARIES})
endif()

# Enable CTest testing.
enable_testing()

# Define the test files and their requirements.
file(GLOB_RECURSE TEST_SOURCES test/main.c test/mock_io/*.c src/chip8.c src/io.c src/app.c)

# Add a test_chip8 target.
add_executable(test_chip8 ${TEST_SOURCES})
add_dependencies(test_chip8 chip8)
target_link_libraries(test_chip8 ${CMAKE_THREAD_LIBS_INIT})

# Add a test running the actual unit tests through CMake.
add_test(test test_chip8)

# Add an autoconf-like `make check`.
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test_chip8)

# Define an install target for the chip8 executable.
install(TARGETS chip8 RUNTIME DESTINATION bin)

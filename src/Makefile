CC     = gcc
CFLAGS = -ggdb -Wall -Wextra

RM     = rm

chip8-debug: *.c */*.c
	$(CC) $(CFLAGS) -o $@ $^ -lSDL -DCHIP8_DEBUG

chip8: *.c */*.c
	$(CC) $(CFLAGS) -o $@ $^ -lSDL

clean:
	@([ -e chip8 ] && $(RM) chip8)

.PHONY: chip8 clean

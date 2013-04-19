CC     = gcc
CFLAGS = -ggdb -Wall -Wextra

RM     = rm

chip8-debug: *.c */*.c
	$(CC) $(CFLAGS) -o $@ $^ -lSDL -DCHIP8_DEBUG

chip8: *.c */*.c
	$(CC) $(CFLAGS) -o $@ $^ -lSDL

clean:
	@([ -e chip8 ] && $(RM) chip8)
	@([ -e chip8-debug ] && $(RM) chip8-debug)

.PHONY: chip8-debug chip8 clean

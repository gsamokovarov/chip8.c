CC     = gcc
CFLAGS = -O3 -Wall -Wextra

RM     = rm

chip8: *.c */*.c
	$(CC) $(CFLAGS) -o $@ $^ -lSDL

clean:
	@([ -e chip8 ] && $(RM) chip8)

.PHONY: chip8 clean

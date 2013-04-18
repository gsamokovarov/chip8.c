CC     = gcc
CFLAGS = -ggdb -Wall -Wextra

RM     = rm

chip8: *.c */*.c
	$(CC) $(CFLAGS) -o $@ $^ -lSDL -DCHIP8_DEBUG

clean:
	@([ -e chip8 ] && $(RM) chip8)

.PHONY: chip8 clean

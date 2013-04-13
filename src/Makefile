CC     = gcc
CFLAGS = -O3 -Wall -Wextra

RM     = rm

chip8: *.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@([ -e chip8 ] && $(RM) chip8)

.PHONY: chip8 clean

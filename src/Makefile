CC     = gcc
CFLAGS = -O2 -Wall -Wextra

RM     = rm

chip8: *.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	$(RM) chip8

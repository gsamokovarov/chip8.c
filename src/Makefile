CC     = gcc
CFLAGS = -O2 -Wall -Wextra

RM     = rm

chip8: *.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@$(RM) chip8

.PHONY: clean

CC     = gcc
CFLAGS = -O2 -Wall -Wextra

RM     = rm

immediate: test
	@(./$^)

test: *.c
	$(CC) $(CFLAGS) -o $@ $^ ../src/chip8.h

.PHONY: clean immediate

clean:
	$(RM) test

CC     = gcc
CFLAGS = -ggdb -Wall -Wextra

RM     = rm

immediate: test
	@(./$^)

test: *.c
	$(CC) $(CFLAGS) -o $@ $^ ../src/chip8.c

clean:
	@([ -e test ] && $(RM) test)

.PHONY: immediate test clean

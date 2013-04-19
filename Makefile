compile-with-debug:
	@(cd src; make chip8-debug)

compile:
	@(cd src; make chip8)

test:
	@(cd test; make immediate)

clean:
	@(cd src; make clean)
	@(cd test; make clean)

.PHONY: test compile clean

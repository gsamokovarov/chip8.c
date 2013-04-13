test:
	@(cd test; make immediate)

compile:
	@(cd src; make chip8)

clean:
	@(cd src; make clean)
	@(cd test; make clean)

.PHONY: test

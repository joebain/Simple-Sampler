.PHONY: all
all:
	@cd build && $(MAKE) --no-print-directory && cd ..

.PHONY: clean
clean:
	@cd build && $(MAKE) --no-print-directory clean && cd ..

.PHONY: run
run: all
	@bin/simple-sampler

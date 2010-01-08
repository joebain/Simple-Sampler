.PHONY: all
all:
	@cd build && $(MAKE) --no-print-directory

.PHONY: clean
clean:
	@cd build && $(MAKE) --no-print-directory clean

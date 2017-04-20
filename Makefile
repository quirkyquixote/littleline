
.PHONY: all
all:
	@make -C src all

.PHONY: clean
clean:
	@make -C src clean

.PHONY: install
install:
	@make -C src install

.PHONY: examples
examples: all
	@make -C examples $@

.PHONY: clean-examples
clean-examples:
	@make -C examples $@

.PHONY: install-examples
install-examples: install
	@make -C examples $@

.PHONY: test
test: all
	@make -C tests all

.PHONY: clean-test
clean-test:
	@make -C tests clean

.PHONY: realclean
realclean:
	git clean -fdx


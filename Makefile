
include config.mk

OBJECTS += binding.o
OBJECTS += buffer.o
OBJECTS += history.o
OBJECTS += littleline.o

LIBS = liblittleline.so
INSTALL_LIBS = $(addprefix $(libdir)/,$(LIBS))

.PHONY: all
all: $(LIBS)

.PHONY: clean
clean:
	@$(RM) $(OBJECTS)
	@$(RM) $(LIBS)

.PHONY: install
install: all $(INSTALL_LIBS)

.PHONY: examples
examples: all
	@make -C examples $@

.PHONY: clean-examples
clean-examples:
	@make -C examples $@

.PHONY: install-examples
install-examples: install
	@make -C examples $@

.PHONY: build-test
build-test: all
	@make -C tests $@

.PHONY: test
test: build-test
	@make -C tests $@

.PHONY: clean-test
clean-test:
	@make -C tests $@

.PHONY: realclean
realclean: clean clean-test clean-examples

liblittleline.so: $(OBJECTS)


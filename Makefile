
include config.mk

OBJECTS += binding.o
OBJECTS += buffer.o
OBJECTS += history.o
OBJECTS += littleline.o

LIBS += liblittleline.so
LIBS += liblittleline.a

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

.PHONY: test
test: all
	@make -C tests all

.PHONY: clean-test
clean-test:
	@make -C tests clean

.PHONY: realclean
realclean:
	git clean -fdx

liblittleline.so: $(OBJECTS)
liblittleline.a: $(OBJECTS)



include config.mk

LITTLELINE_OBJECTS += binding.o
LITTLELINE_OBJECTS += buffer.o
LITTLELINE_OBJECTS += history.o
LITTLELINE_OBJECTS += littleline.o

.PHONY: all
all: liblittleline.so

.PHONY: clean
clean:
	@$(RM) *.o
	@$(RM) liblittleline.so

.PHONY: install
install: all
	$(call colorecho,Installing $(LIBDIR)/liblittleline.so.$(VERSION))
	@$(INSTALL) liblittleline.so $(LIBDIR)/liblittleline.so.$(VERSION)
	$(call colorecho,Installing $(LIBDIR)/liblittleline.so)
	@cd $(LIBDIR) && ln -s -f liblittleline.so.$(VERSION) liblittleline.so

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
realclean: clean clean-test

liblittleline.so: $(LITTLELINE_OBJECTS)
	$(call colorecho,Linking C shared library $@)
	@$(CC) -shared -Wl,-soname,liblittleline.so.$(VERSION) -o liblittleline.so $^



include ../config.mk

OBJECTS += buffer.o
OBJECTS += binding.o
OBJECTS += history.o

PROGRAMS += buffer
PROGRAMS += binding
PROGRAMS += history

TESTS += test_buffer
TESTS += test_binding
TESTS += test_history

.PHONY: build-test
build-test: $(PROGRAMS)

.PHONY: test
test: $(TESTS)

.PHONY: clean-test
clean-test:
	@$(RM) $(OBJECTS)
	@$(RM) $(PROGRAMS)

buffer: buffer.o ../buffer.o
binding: binding.o ../binding.o
history: history.o ../history.o ../buffer.o

test_%: %
	@echo TEST $<; ./$<


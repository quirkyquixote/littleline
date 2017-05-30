
include ../config.mk

QUIET_TEST = @echo TEST $@;

MEMCHECK = valgrind -q --tool=memcheck

tests += buffer_output
tests += binding_output
tests += history_output
tests += buffer_memcheck
tests += binding_memcheck
tests += history_memcheck

.PHONY: all
all: $(tests)

.PHONY: clean
clean:
	$(RM) buffer binding history
	$(RM) *.o
	$(RM) *.log

.PHONY: buffer_output
buffer_output: buffer
	$(QUIET_TEST)./$<

.PHONY: binding_output
binding_output: binding
	$(QUIET_TEST)./$<

.PHONY: history_output
history_output: history
	$(QUIET_TEST)./$<

.PHONY: buffer_memcheck
buffer_memcheck: buffer
	$(QUIET_TEST)$(MEMCHECK) ./$<

.PHONY: binding_memcheck
binding_memcheck: binding
	$(QUIET_TEST)$(MEMCHECK) ./$<

.PHONY: history_memcheck
history_memcheck: history
	$(QUIET_TEST)$(MEMCHECK) ./$<

buffer: buffer.o ../src/liblittleline.a
binding: binding.o ../src/liblittleline.a
history: history.o ../src/liblittleline.a

../src/liblittleline.a:
	@make -C ../src liblittleline.a

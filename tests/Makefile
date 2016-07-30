
include ../config.mk

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
	@$(RM) *.o
	@$(RM) $(PROGRAMS)

buffer: buffer.o ../buffer.o
	$(call colorecho,Linking C executable $@)
	@$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

binding: binding.o ../binding.o
	$(call colorecho,Linking C executable $@)
	@$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

history: history.o ../history.o ../buffer.o
	$(call colorecho,Linking C executable $@)
	@$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

test_%: %
	$(call colorecho,Test $<)
	@./$<


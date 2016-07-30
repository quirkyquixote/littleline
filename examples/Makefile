
include ../config.mk

PROGRAMS += llsh

.PHONY: examples
examples: $(PROGRAMS)

.PHONY: clean-examples
clean-examples:
	@$(RM) *.o
	@$(RM) $(PROGRAMS)

.PHONY: install-examples
install-examples: examples
	$(call colorecho,Installing $(BINDIR)/llsh)
	@$(INSTALL) llsh $(BINDIR)

llsh: llsh.o
	$(call colorecho,Linking C executable $@)
	@$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^ -L.. -llittleline

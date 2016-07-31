
include ../config.mk

OBJECTS += llsh.o

PROGRAMS += llsh

INSTALL_PROGRAMS = $(addprefix $(bindir)/,$(PROGRAMS))

.PHONY: examples
examples: $(PROGRAMS)

.PHONY: clean-examples
clean-examples:
	@$(RM) $(OBJECTS)
	@$(RM) $(PROGRAMS)

.PHONY: install-examples
install-examples: examples $(INSTALL_PROGRAMS)

llsh: llsh.o
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^ -L.. -llittleline

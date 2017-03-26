
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

llsh: llsh.o
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^ ../liblittleline.a

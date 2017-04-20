
include ../config.mk

OBJECTS += llsh.o

PROGRAMS += llsh

INSTALL_PROGRAMS = $(addprefix $(bindir)/,$(PROGRAMS))

.PHONY: all
all: $(PROGRAMS)

.PHONY: clean
clean:
	@$(RM) $(OBJECTS)
	@$(RM) $(PROGRAMS)

llsh: llsh.o ../src/liblittleline.a
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

../src/littleline.a:
	@make -C ../src liblittleline.a


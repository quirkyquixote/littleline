
CC = cc
INSTALL = install
RM = rm -f

version = 1.0
prefix = /usr/local
libdir = $(prefix)/lib

CPPFLAGS =

CFLAGS = -g -Werror -Wfatal-errors

LDFLAGS =

ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(LDFLAGS)

ALL_CFLAGS += -std=c90
ALL_CFLAGS += -pedantic
ALL_CFLAGS += -fPIC

QUIET_CC = @echo CC $@;
QUIET_LINK = @echo LINK $@;
QUIET_INSTALL = @echo INSTALL $@;
QUIET_GEN = @echo GEN $@;

%: %.c 

%: %.o
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

%.o: %.c
	$(QUIET_CC)$(CC) $(ALL_CFLAGS) -c $<

lib%.so: %.o
	$(QUIET_LINK)$(CC) -shared -Wl,-soname,$@.$(version) -o $@ $^

$(DESTDIR)$(libdir)/%.so: %.so
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL) $< $@.$(version)
	@cd $(DESTDIR)$(libdir) && ln -s -f $<.$(version) $<


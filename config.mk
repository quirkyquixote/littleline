
CC = cc
INSTALL = install
RM = rm -f

version = 1.0
prefix = $(HOME)
libdir = $(prefix)/lib

CPPFLAGS =

CFLAGS = -ansi \
	 -pedantic \
	 -Werror \
	 -Wfatal-errors

LDFLAGS =

ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(LDFLAGS)

ALL_CFLAGS += -fPIC

define colorecho
      @tput setaf 7
      @echo $1
      @tput sgr0
endef

QUIET_CC = @$(call colorecho, CC $@);
QUIET_LINK = @$(call colorecho, LINK $@);
QUIET_INSTALL = @$(call colorecho, INSTALL $@);

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


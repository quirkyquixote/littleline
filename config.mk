
CC = cc
INSTALL = install
RM = rm -f

VERSION = 1.0
PREFIX = $(HOME)
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib

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

%.o: %.c
	$(call colorecho,Building C object $@)
	@$(CC) $(ALL_CFLAGS) -c $<

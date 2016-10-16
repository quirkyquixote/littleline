
include ../config.mk

OBJS += buffer.o
OBJS += binding.o
OBJS += history.o

PROGRAMS = $(OBJS:.o:)
DEPS = $(OBJS:.o=.d)
LOGS = $(OBJS:.o=.log)

.PHONY: all
all: $(LOGS)
	@for i in $(LOGS); do echo TEST $$i; diff -q $$i $$i.gold; done

.PHONY: clean
clean:
	$(RM) $(PROGRAMS)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(LOGS)

buffer: buffer.o ../buffer.o
binding: binding.o ../binding.o
history: history.o ../history.o ../buffer.o

%.log: %
	$(QUIET_GEN)./$< > $@ 2>&1



include ../config.mk

DIFF = diff -q --ignore-blank-lines --ignore-space-change
MEMCHECK = valgrind -q --tool=memcheck

objs += buffer.o
objs += binding.o
objs += history.o

bins = $(objs:.o=)
deps = $(objs:.o=.d)
logs = $(objs:.o=.log)

.PHONY: all
all: $(logs)
	@for i in $(bins); do echo DIFF $$i.log; $(DIFF) $$i.log $$i.gold; done
	@for i in $(bins); do echo MEMCHECK $$i; $(MEMCHECK) ./$$i > /dev/null 2>&1; done

.PHONY: clean
clean:
	$(RM) $(bins)
	$(RM) $(objs)
	$(RM) $(deps)
	$(RM) $(logs)

buffer: buffer.o ../src/liblittleline.a
binding: binding.o ../src/liblittleline.a
history: history.o ../src/liblittleline.a

%.log: % make-always
	$(QUIET_GEN)./$< > $@ 2>&1

../src/littleline.a:
	@make -C ../src liblittleline.a

.PHONY: make-always
make-always:

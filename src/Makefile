
include ../config.mk

objs += binding.o
objs += buffer.o
objs += history.o
objs += littleline.o

deps = $(objs:.o=.d)

libs = liblittleline.so liblittleline.a
install_libs = $(addprefix $(libdir)/,$(libs))

headers += binding.h
headers += buffer.h
headers += history.h
headers += littleline.h

install_headers = $(addprefix $(includedir)/,$(headers))

.PHONY: all
all: $(libs) $(bins)

.PHONY: clean
clean:
	$(RM) $(objs)
	$(RM) $(libs)
	$(RM) $(deps)

.PHONY: install
install: all $(install_libs) $(install_headers)

liblittleline.so: $(objs)
liblittleline.a: $(objs)

-include $(deps)

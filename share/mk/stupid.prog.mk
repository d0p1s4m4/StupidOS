# File: stupid.prog.mk
# Building programs from source files
#
# targets:
# - all:
#   	build the program and its manual page.
# - clean:
#   	remove the program and any object files.
# - install:
#		install the program and its manual page.

include $(TOPDIR)/share/mk/stupid.own.mk

ifdef PROG

CLEANFILES += $(PROG)

ifdef SRCS

OBJS	+= $(addsuffix .o, $(basename $(SRCS)))

CLEANFILES += $(OBJS)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDADD)

else

CLEANFILES += $(PROG).o

$(PROG): $(PROG).o
	$(CC) $(CFLAGS) -o $@ $< $(LDADD)

endif

all: $(PROG)

install:: $(PROG)
	$(INSTALL) -d $(DESTDIR)$(LIBDIR)
	$(INSTALL) $< $(DESTDIR)$(LIBDIR)

endif

include $(TOPDIR)/share/mk/stupid.clean.mk
include $(TOPDIR)/share/mk/stupid.sys.mk

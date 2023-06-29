# File: stupid.lib.mk
# Support for building libraries

include $(TOPDIR)/share/mk/stupid.own.mk

ifdef LIB

CLEANFILES += lib$(LIB).a

ifdef SRCS

OBJS	+= $(addsuffix .o, $(basename $(SRCS)))

CLEANFILES	+= $(OBJS)

lib$(LIB).a: $(OBJS)
	$(AR) rcs $@ $^

else

CLEANFILES += $(LIB).o

lib$(LIB).a: $(LIB).o
	$(AR) rcs $@ $^

endif

all: lib$(LIB).a

endif

include $(TOPDIR)/share/mk/stupid.clean.mk
include $(TOPDIR)/share/mk/stupid.sys.mk

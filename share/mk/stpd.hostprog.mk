include stpd.base.mk

ifdef HOSTPROG

ifndef SRCS
SRCS=$(HOSTPROG).c
endif

ifndef OBJS
OBJS=$(SRCS:.c=.lo)
endif

_HOSTPROGINST = $(addprefix $(TOOLSDIR)/bin/,$(HOSTPROG))

$(HOSTPROG): $(OBJS)
	$(MSG_LINK)
	@$(HOST_CC) -o $@ $^ $(HOST_LDFLAGS) $(LDADD)

all: $(HOSTPROG)

install: $(_HOSTPROGINST)

$(_HOSTPROGINST): $(HOSTPROG)
	$(MSG_INSTALL)
	@install -D $< $@

CLEANFILES += $(OBJS) $(HOSTPROG)

include stpd.clean.mk

endif

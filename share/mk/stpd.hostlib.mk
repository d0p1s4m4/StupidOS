include stpd.base.mk

ifdef HOSTLIB

ifndef OBJS
OBJS=$(SRCS:.c=.lo)
endif

_HOSTLIBINST = $(addprefix $(TOOLSDIR)/lib/,$(HOSTLIB))

$(HOSTLIB): $(OBJS)
	$(MSG_BUILD)
	@$(HOST_AR) rcs $@ $<

all: $(HOSTLIB)

install: $(_HOSTLIBINST)

$(_HOSTLIBINST): $(HOSTLIB)
	$(MSG_INSTALL)
	@install -D $< $@

CLEANFILES += $(OBJS) $(HOSTLIB)

include stpd.clean.mk

endif

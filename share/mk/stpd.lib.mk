include stpd.base.mk

ifdef LIB

_LIB:=lib$(LIB).a
_LIBINST=$(addprefix $(DESTDIR)$(LIBDIR)/, $(_LIB))

all: $(_LIB)

$(_LIB): $(OBJS)
	$(MSG_BUILD)
	@$(AR) rcs $@ $^

install: $(_LIBINST)

$(_LIBINST): $(_LIB)
	$(MSG_INSTALL)
	@install -D $< $@

CLEANFILES += $(_LIB) $(OBJS)

include stpd.inc.mk
include stpd.man.mk
include stpd.clean.mk

endif

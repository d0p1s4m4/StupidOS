include stpd.base.mk

ifdef MAN

_MSECTIONS= 1 2 3 4 5 6 7 8 9
_MANINST=$(foreach S,$(_MSECTIONS),$(addprefix $(DESTDIR)$(MANDIR)/$S/,$(filter %.$S,$(MAN))))

endif

include stpd.base.mk

ifdef MAN

define maninstall-rule

$1/$2: $2
	$$(MSG_INSTALL)
	@install -D $2 $1/$2

endef

_MSECTIONS= 1 2 3 4 5 6 7 8 9
_MANINST=$(foreach S,$(_MSECTIONS),$(addprefix $(DESTDIR)$(MANDIR)/$S/,$(filter %.$S,$(MAN))))

maninstall: $(_MANINST)

$(eval $(foreach S,$(_MSECTIONS), \
	$(foreach M,$(filter %.$S,$(MAN)), \
		$(call maninstall-rule,$(DESTDIR)$(MANDIR)/$S,$M))))

install: maninstall

endif

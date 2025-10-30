include stpd.base.mk

FILESDIR?= $(BINDIR)

ifdef FILES

$(DESTDIR)$(FILESDIR)/%: %
	$(MSG_INSTALL)
	@install -D $< $@

install: $(addprefix $(DESTDIR)$(FILESDIR)/, $(FILES))

endif

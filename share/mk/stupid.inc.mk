include $(TOPDIR)/share/mk/stupid.own.mk

ifdef INCS

install::
	$(INSTALL) -d $(DESTDIR)$(INCSDIR)
	$(INSTALL) $^ $(DESTDIR)$(INCSDIR)

endif

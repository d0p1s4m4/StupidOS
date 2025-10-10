include stpd.base.mk

ifdef MODULE

all: $(MODULE).mod

%.mod: %.asm
	$(MSG_BUILD)
	@fasm $^ $@ >/dev/null

$(DESTDIR)/$(MODULE).mod: $(MODULE).mod
	$(MSG_INSTALL)
	@install -D $< $@

.PHONY: install
install: $(DESTDIR)/$(MODULE).mod

CLEANFILES += $(MODULE).mod

include stpd.clean.mk

endif

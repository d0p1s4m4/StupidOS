include stpd.base.mk

MODULE = dummy.mod

all: $(MODULE)

$(MODULE): $(MODULE:.mod=.asm)
	$(MSG_BUILD)
	@fasm $^ $@

$(DESTDIR)/$(MODULE): $(MODULE)
	$(MSG_INSTALL)
	@install -D $< $@

.PHONY: install
install: $(DESTDIR)/$(MODULE)

CLEANFILES = $(MODULE)

include stpd.clean.mk

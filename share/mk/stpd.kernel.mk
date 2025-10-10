include stpd.base.mk

ifdef KERNEL

.PHONY: all
all: $(KERNEL).sys $(KERNEL).sym

$(KERNEL).sys $(KERNEL).fas &: $(SRCS)
	$(MSG_BUILD)
	@$(FASM) -s $(KERNEL).fas $< $(KERNEL).sys >/dev/null

$(KERNEL).sym: $(KERNEL).fas
	$(MSG_CREATE)
	@$(FAS2SYM) -o $@ $<

.PHONY: install
install: $(DESTDIR)/$(KERNEL).sys $(SYMSDIR)/$(KERNEL).sym

$(DESTDIR)/$(KERNEL).sys: $(KERNEL).sys
	$(MSG_INSTALL)
	@install -D $< $@

$(SYMSDIR)/$(KERNEL).sym: $(KERNEL).sym
	$(MSG_INSTALL)
	@install -D $< $@

CLEANFILES += $(KERNEL).sys $(KERNEL).fas $(KERNEL).sys

include stpd.inc.mk
include stpd.clean.mk

endif

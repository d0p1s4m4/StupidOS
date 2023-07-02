.DEFAULT_GOAL := all
TOPDIR	:= $(CURDIR)
export TOPDIR

SUBDIR := thirdparty lib bin kernel

CLEANFILES += stupid.iso stupid.tar.gz

.PHONY: docs
docs:
	-mkdir -p docs/html
	naturaldocs -p docs/config -img docs/img -xi tmp -i . -o HTML docs/html
	cp docs/img/favicon.ico docs/html/

.PHONY: stupid.iso
stupid.iso:
	$(MAKE) all
	DESTDIR=$(TOPDIR)/sysroot $(MAKE) install
	$(TOPDIR)/tools/create-iso $@ sysroot

.PHONY: stupid.tar.gz
stupid.tar.gz:
	$(MAKE) all
	DESTDIR=$(TOPDIR)/sysroot $(MAKE) install
	tar -czvf $@ sysroot

run: stupid.iso
	qemu-system-i386 -cdrom $< -serial stdio

include $(TOPDIR)/share/mk/stupid.subdir.mk
include $(TOPDIR)/share/mk/stupid.clean.mk

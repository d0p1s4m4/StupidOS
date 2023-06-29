.DEFAULT_GOAL := all
TOPDIR	:= $(CURDIR)
export TOPDIR

SUBDIR := thirdparty lib bin kernel

.PHONY: docs
docs:
	-mkdir -p docs/html
	naturaldocs -p docs/config -img docs/img -xi tmp -i . -o HTML docs/html
	cp docs/img/favicon.ico docs/html/

include $(TOPDIR)/share/mk/stupid.subdir.mk

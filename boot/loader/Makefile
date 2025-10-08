include stpd.base.mk

COMMON_DIR = ../common
SYS_DIR    = ../../kernel/sys

COMMON_SRCS = const.inc \
				bootinfo.inc

SYS_SRCS = bootinfo.inc \
			register.inc

LOADER_SRCS	= loader.asm \
				$(addprefix $(COMMON_DIR)/, $(COMMON_SRCS)) \
				$(addprefix $(SYS_DIR)/, $(SYS_SRCS)) \
				fat.inc \
				video.inc \
				memory.inc \
				logger.inc \
				a20.inc \
				multiboot.inc \
				stpdfs.inc \
				disk.inc

.PHONY: all
all: stpdldr.sys

stpdldr.sys: $(LOADER_SRCS)
	$(MSG_BUILD)
	@$(FASM) loader.asm $@ >/dev/null

%.inc: %.inc.in
	@$(VERSION_SH)

.PHONY: install
install: $(DESTDIR)/stpdldr.sys

$(DESTDIR)/stpdldr.sys: stpdldr.sys
	$(MSG_INSTALL)
	@install -D $< $@

CLEANFILES = stpdldr.sys $(COMMON_DIR)/const.inc

include stpd.clean.mk

include stpd.base.mk

COMMON_DIR = ../common

COMMON_SRCS = const.inc \
				macro.inc


BOOTIA32_EFI_SRCS = bootia32.asm \
					uefi.inc \
					logger.inc \
					memory.inc \
					fs.inc \
					$(addprefix $(COMMON_DIR)/, $(COMMON_SRCS))

.PHONY: all
all: BOOTIA32.EFI

%.inc: %.inc.in
	@$(VERSION_SH)

BOOTIA32.EFI: $(BOOTIA32_EFI_SRCS)
	$(MSG_BUILD)
	@$(FASM) bootia32.asm $@ >/dev/null

.PHONY: install
install: $(DESTDIR)/EFI/BOOT/BOOTIA32.EFI

$(DESTDIR)/EFI/BOOT/BOOTIA32.EFI: BOOTIA32.EFI 
	$(MSG_INSTALL)
	@install -D $< $@

CLEANFILES = BOOTIA32.EFI $(COMMON_DIR)/const.inc

include stpd.clean.mk

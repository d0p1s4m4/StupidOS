include stpd.base.mk

BINS = boot_floppy1440.bin \
		boot_floppy2880.bin \
		boot_mbr.bin \
		boot_hdd.bin

COMMON_DIR = ../common

COMMON_SRCS = const.inc \
				fat12.inc 

FLOPPY_SRCS	= floppy.asm \
			$(addprefix $(COMMON_DIR)/, $(COMMON_SRCS))

HDD_SRCS = hdd.asm \
			$(addprefix $(COMMON_DIR)/, $(COMMON_SRCS))

MBR_SRCS = mbr.asm \
			$(addprefix $(COMMON_DIR)/, $(COMMON_SRCS))

.PHONY: all
all: $(BINS)

boot_floppy1440.bin: $(FLOPPY_SRCS)
	$(MSG_BUILD)
	@$(FASM) floppy.asm $@ >/dev/null

boot_floppy2880.bin: $(FLOPPY_SRCS)
	$(MSG_BUILD)
	@$(FASM) -DFLOPPY_SIZE=FLOPPY_2880 floppy.asm $@ >/dev/null

boot_mbr.bin: $(MBR_SRCS)
	$(MSG_BUILD)
	@$(FASM) mbr.asm $@ >/dev/null

boot_hdd.bin: $(HDD_SRCS)
	$(MSG_BUILD)
	@$(FASM) hdd.asm $@ >/dev/null

.PHONY: install
install: $(BINS)

CLEANFILES = $(BINS)

include stpd.clean.mk

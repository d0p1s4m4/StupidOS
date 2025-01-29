TARGET = boot_floppy1440.bin \
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
all: $(TARGET)

%.inc: %.inc.in
	sh $(TOOLSDIR)/version.sh $< $@

boot_floppy1440.bin: $(FLOPPY_SRCS)
	$(AS) floppy.asm $@

boot_floppy2880.bin: $(FLOPPY_SRCS)
	$(AS) -DFLOPPY_SIZE=FLOPPY_2880 floppy.asm $@

boot_mbr.bin: $(MBR_SRCS)
	$(AS) mbr.asm $@

boot_hdd.bin: $(HDD_SRCS)
	$(AS) hdd.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)

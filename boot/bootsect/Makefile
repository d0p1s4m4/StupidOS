TARGET = boot_floppy1440.bin \
		boot_floppy2880.bin \
		boot_mbr.bin \
		boot_hdd.bin

FLOPPY_SRCS	= floppy.asm \
			../common/const.inc \
			../common/fat12.inc

HDD_SRCS = hdd.asm \
			../common/const.inc \
			../common/fat12.inc

MBR_SRCS = mbr.asm \
			../common/const.inc \
			../common/fat12.inc

.PHONY: all
all: $(TARGET)

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

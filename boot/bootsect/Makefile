TARGET = bootsector.bin

FLOPPY_SRCS	= floppy.asm \
			../common/const.inc \
			../common/fat12.inc

.PHONY: all
all: $(TARGET)

bootsector.bin: $(FLOPPY_SRCS)
	$(AS) floppy.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)

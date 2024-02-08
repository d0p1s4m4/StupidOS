AS = fasm
RM = rm

TARGET = bootsector.bin stpdboot.sys

STAGE0_SRCS	= boot0.asm \
			const.inc \
			fat12.inc

STAGE1_SRCS	= boot1.asm \
			const.inc \
			a20.inc

.PHONY: all
all: $(TARGET)

bootsector.bin: $(STAGE0_SRCS)
	$(AS) boot0.asm $@

stpdboot.sys: $(STAGE1_SRCS)
	$(AS) boot1.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)
	@ mkdir -p $(DESTDIR)

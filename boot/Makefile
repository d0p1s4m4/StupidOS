AS = fasm
RM = rm

TARGET = bootsector.bin stpdboot.sys bootia32.efi

STAGE0_SRCS	= boot0.asm \
			const.inc \
			fat12.inc

STAGE1_SRCS	= boot1.asm \
			const.inc \
			a20.inc \
			multiboot.inc

BOOTIA32_EFI_SRCS = bootia32.asm \
					uefi.inc

.PHONY: all
all: $(TARGET)

bootsector.bin: $(STAGE0_SRCS)
	$(AS) boot0.asm $@

stpdboot.sys: $(STAGE1_SRCS)
	$(AS) boot1.asm $@

bootia32.efi: $(BOOTIA32_EFI_SRCS)
	$(AS) bootia32.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)
	@ mkdir -p $(DESTDIR)
	install stpdboot.sys $(DESTDIR)
	@ mkdir -p $(DESTDIR)/EFI/BOOT
	install bootia32.efi $(DESTDIR)/EFI/BOOT

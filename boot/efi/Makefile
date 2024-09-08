TARGET = BOOTIA32.EFI

BOOTIA32_EFI_SRCS = bootia32.asm \
					uefi.inc \
					logger.inc \
					memory.inc \
					fs.inc \
					../common/const.inc \
					../common/macro.inc

.PHONY: all
all: $(TARGET)

BOOTIA32.EFI: $(BOOTIA32_EFI_SRCS)
	$(AS) bootia32.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)
	@ mkdir -p $(DESTDIR)/EFI/BOOT
	install $(TARGET) $(DESTDIR)/EFI/BOOT

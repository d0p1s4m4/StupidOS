TARGET = BOOTIA32.EFI


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
all: $(TARGET)

%.inc: %.inc.in
	sh $(TOOLSDIR)/version.sh $< $@

BOOTIA32.EFI: $(BOOTIA32_EFI_SRCS)
	$(AS) bootia32.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)
	@ mkdir -p $(DESTDIR)/EFI/BOOT
	install $(TARGET) $(DESTDIR)/EFI/BOOT

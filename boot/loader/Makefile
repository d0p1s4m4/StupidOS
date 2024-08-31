TARGET = stpdldr.sys

LOADER_SRCS	= loader.asm \
				../common/const.inc \
				../common/bootinfo.inc \
				../../kernel/sys/bootinfo.inc \
				../../kernel/sys/register.inc \
				fat.inc \
				video.inc \
				memory.inc \
				logger.inc \
				a20.inc \
				multiboot.inc \
				stpdfs.inc

.PHONY: all
all: $(TARGET)


stpdldr.sys: $(LOADER_SRCS)
	$(AS) loader.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)
	@ mkdir -p $(DESTDIR)
	install $(TARGET) $(DESTDIR)

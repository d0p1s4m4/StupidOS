TARGET = stpdldr.sys

LOADER_SRCS	= loader.asm \
				../common/const.inc \
				a20.inc \
				multiboot.inc

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

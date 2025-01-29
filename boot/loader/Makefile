TARGET = stpdldr.sys

COMMON_DIR = ../common
SYS_DIR    = ../../kernel/sys

COMMON_SRCS = const.inc \
				bootinfo.inc

SYS_SRCS = bootinfo.inc \
			register.inc

LOADER_SRCS	= loader.asm \
				$(addprefix $(COMMON_DIR)/, $(COMMON_SRCS)) \
				$(addprefix $(SYS_DIR)/, $(SYS_SRCS)) \
				fat.inc \
				video.inc \
				memory.inc \
				logger.inc \
				a20.inc \
				multiboot.inc \
				stpdfs.inc \
				disk.inc

.PHONY: all
all: $(TARGET)

%.inc: %.inc.in
	sh $(TOOLSDIR)/version.sh $< $@

stpdldr.sys: $(LOADER_SRCS)
	$(AS) loader.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)
	@ mkdir -p $(DESTDIR)
	install $(TARGET) $(DESTDIR)

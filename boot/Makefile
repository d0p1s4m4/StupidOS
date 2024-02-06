AS = fasm
RM = rm

TARGET = bootcode.bin

SRCS	= boot.asm \
			const.inc \
			a20.inc

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRCS)
	$(AS) boot.asm $@

.PHONY: clean
clean:
	$(RM) $(TARGET)

.PHONY: install
install: $(TARGET)
	@ mkdir -p $(DESTDIR)

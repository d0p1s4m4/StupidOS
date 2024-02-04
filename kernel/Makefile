AS	= fasm
RM	= rm -f
INSTALL = install

KERNEL	= vmstupid
SRCS	= kernel.asm \
			const.inc \
			boot/multiboot.inc \
			boot/boot.inc \
			mm/mm.inc

.PHONY: all
all: $(KERNEL)

$(KERNEL): $(SRCS)
	$(AS) kernel.asm $@

.PHONY: clean
clean:
	$(RM) $(KERNEL)

.PHONY: install
install: $(KERNEL)
	@ mkdir -p $(DESTDIR)
	install $< $(DESTDIR)

.PHONY: all clean
AS	= fasm
RM	= rm -f
INSTALL = install

KERNEL	= vmstupid.sys
SRCS	= kernel.asm \
			const.inc \
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

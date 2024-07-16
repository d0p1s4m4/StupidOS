AS	= fasm
RM	= rm -f
INSTALL = install

KERNEL	= vmstupid.sys
SRCS	= kernel.asm \
			const.inc \
			klog.inc \
			mm/mm.inc \
			mm/pmm.inc \
			lock.inc \
			gdt.inc \
			isr.inc \
			pic.inc \
			idt.inc \
			dev/console.inc \
			dev/at/pit.inc \
			dev/at/cga.inc \
			dev/at/kbd.inc \
			dev/at/floppy.inc \
			dev/dev.inc \
			fs/fat.inc \
			fs/stpdfs.inc \
			fs/xv6fs.inc \
			vfs.inc

.PHONY: all
all: $(KERNEL)

PHONY: const.inc
const.inc: const.inc.in
	sh $(TOOLSDIR)/version.sh $< $@

$(KERNEL): $(SRCS)
	$(AS) kernel.asm $@

.PHONY: clean
clean:
	$(RM) $(KERNEL)

.PHONY: install
install: $(KERNEL)
	@ mkdir -p $(DESTDIR)
	install $< $(DESTDIR)

.PHONY: all clean install

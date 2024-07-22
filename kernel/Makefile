AS	= fasm
RM	= rm -f
INSTALL = install

ASMINCS = sys/bootinfo.inc \
			sys/coff.inc \
			sys/cpu.inc \
			sys/errno.inc \
			sys/macro.inc \
			sys/mmu.inc \
			sys/process.inc \
			sys/register.inc

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
			heap.inc \
			dev/console.inc \
			dev/at/pit.inc \
			dev/at/cga.inc \
			dev/at/kbd.inc \
			dev/at/com.inc \
			dev/at/ne2k.inc \
			dev/at/ata.inc \
			dev/at/floppy.inc \
			dev/dev.inc \
			fs/fat.inc \
			fs/stpdfs.inc \
			fs/xv6fs.inc \
			vfs.inc \
			proc.inc \
			$(ASMINCS)

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
	install $(KERNEL) $(DESTDIR)
	@ mkdir -p $(DESTDIR)$(ASMDIR)/sys
	install $(ASMINCS) $(DESTDIR)$(ASMDIR)/sys

.PHONY: all clean install

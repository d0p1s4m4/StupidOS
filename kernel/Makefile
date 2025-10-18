ASMINCS = sys/bootinfo.inc \
			sys/coff.inc \
			sys/cpu.inc \
			sys/errno.inc \
			sys/macro.inc \
			sys/mmu.inc \
			sys/process.inc \
			sys/register.inc

KERNEL	= vmstupid

MM_SRCS = bootstrap.inc \
			mm.inc \
			pmm.inc \
			vmem.inc

SRCS	= kernel.asm \
			const.inc \
			klog.inc \
			$(addprefix mm/, $(MM_SRCS)) \
			lock.inc \
			gdt.inc \
			isr.inc \
			pic.inc \
			idt.inc \
			heap.inc \
			queue.inc \
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
			bio.inc \
			$(ASMINCS)

FAS2SYMFLAGS = -t kmain,kend

.PHONY: const.inc
const.inc: const.inc.in
	@$(VERSION_SH)

CLEANFILES = const.inc

include stpd.kernel.mk

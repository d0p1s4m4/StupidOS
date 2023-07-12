KERNEL	= vmstupid
SRCS	= boot/head.s boot/feat.s kernel.s gdt.s pic.s isr.s idt.s pmm.s paging.s \
			lib/log.s drivers/serial.s
INCS	= sys/multiboot.inc \
			sys/i386/cpuid.inc \
			sys/i386/mmu.inc \
			sys/i386/registers.inc

include $(TOPDIR)/share/mk/stupid.kernel.mk
#include $(TOPDIR)/share/mk/stupid.inc.mk

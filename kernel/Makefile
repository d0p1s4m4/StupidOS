KERNEL	= vmstupid

SRCS	= boot/head.s \
			boot/gdt.s \
			boot/idt.s \
			boot/isr.s \
			boot/tss.s \
			kernel.s \
			lib/log.s dev/at/serial.s \
			i18n/msg_en.s
INCS	= sys/multiboot.inc \
			sys/i386/cpuid.inc \
			sys/i386/mmu.inc \
			sys/i386/registers.inc

include $(TOPDIR)/share/mk/stupid.kernel.mk
#include $(TOPDIR)/share/mk/stupid.inc.mk

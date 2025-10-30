COMMON_DIR = ../common
SYS_DIR    = ../../kernel/sys

KERNEL = stpdldr

COMMON_SRCS = const.inc \
				bootinfo.inc \
				cfg.inc \
				bios.inc

SYS_SRCS = bootinfo.inc \
			register.inc \
			mmu.inc

SRCS	= loader.asm \
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

FAS2SYMFLAGS = -t _start,_end

include stpd.kernel.mk

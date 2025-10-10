COMMON_DIR = ../common
SYS_DIR    = ../../kernel/sys

KERNEL = stpdldr

COMMON_SRCS = const.inc \
				bootinfo.inc

SYS_SRCS = bootinfo.inc \
			register.inc

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

.PHONY: $(COMMON_DIR)/const.inc
$(COMMON_DIR)/const.inc: $(COMMON_DIR)/const.inc.in
	@$(VERSION_SH)

CLEANFILES = $(COMMON_DIR)/const.inc

include stpd.kernel.mk

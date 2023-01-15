.SUFFIXES:
.DELETE_ON_ERROR:
.DEFAULT_GOAL := all

AS		= nasm
LD		= ld.lld
OBJDUMP = llvm-objdump
RM		= rm -f

BIN_DIR		= bin
DOC_DIR		= doc
KERNEL_DIR	= kernel
LIB_DIR		= lib
TOOLS_DIR	= tools

include $(TOOLS_DIR)/build.mk

ASFLAGS	= -DSTUPID_VERSION=\"$(shell $(GIT-VERSION))\" -Ilib

GARBADGE	= stupid.img

include $(KERNEL_DIR)/build.mk

all: stupid.img

stupid.img: $(KERNEL_BIN) $(KERNEL_DUMP)

#stupid.img: floppy.bin kern.bin
#	mkdosfs -F 12 -C $@ 1440
#	dd status=noxfer conv=notrunc if=floppy.bin of=$@
#	mcopy -i $@ kern.bin ::kern.sys

%.dump: %.elf
	$(OBJDUMP) -D $^ > $@

clean:
	$(RM) $(GARBADGE)

re: clean all

.PHONY: all clean re

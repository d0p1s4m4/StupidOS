.SUFFIXES:
.DELETE_ON_ERROR:
.DEFAULT_GOAL := all

AS		= nasm
LD		= ld.lld
OBJDUMP = llvm-objdump
QEMU	= qemu-system-i386
RM		= rm -f
MKCWD	= mkdir -p $(@D)
INSTALL = install

BIN_DIR		= bin
DOC_DIR		= docs
KERNEL_DIR	= kernel
LIB_DIR		= lib
TOOLS_DIR	= tools

include $(TOOLS_DIR)/build.mk

ASFLAGS	=  -F dwarf -g -DSTUPID_VERSION="\"$(shell $(GIT-VERSION))\"" -Ilib \
	-DBUILD_DATE="\"$(shell date --iso)\"" \
	-DNASM_VERSION="\"$(shell nasm -v)\""
QEMUFLAGS = -serial stdio -no-reboot

GARBADGE	= stupid.iso

include $(KERNEL_DIR)/build.mk
include $(LIB_DIR)/build.mk
include $(BIN_DIR)/build.mk
include $(DOC_DIR)/build.mk

all: stupid.iso

sysroot: $(KERNEL_BIN) $(KERNEL_DUMP) $(LIBS_BIN)
	$(INSTALL) -d $@
	$(INSTALL) -d $@/bin
	$(INSTALL) -d $@/lib
	$(INSTALL) $(KERNEL_BIN) $@
	$(INSTALL) $(LIBS_BIN) $@/lib

stupid.iso: sysroot
	$(CREATE-ISO) $@ $<

run: stupid.iso
	$(QEMU) $(QEMUFLAGS) -cdrom $<

clean:
	$(RM) $(GARBADGE)

re: clean all

.PHONY: all clean re

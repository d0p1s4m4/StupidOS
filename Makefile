TOPDIR     := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
SYSROOTDIR := $(TOPDIR)/sysroot
TOOLSDIR   := $(TOPDIR)/tools

RM = echo

SUBDIRS	:= boot kernel lib bin

TARGET	= stupid.tar.gz floppy_boot.img
ifneq ($(OS),Windows_NT)
TARGET	+= stupid.iso
endif

.PHONY: all
all: $(TARGET)

GOAL:=install
clean: GOAL:=clean

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 $@"
	DESTDIR=$(SYSROOTDIR) $(MAKE) -C $@ $(GOAL)

.PHONY: stupid.iso
stupid.iso: $(SUBDIRS)
	$(TOPDIR)/tools/create-iso $@ sysroot

.PHONY: stupid.tar.gz
stupid.tar.gz: $(SUBDIRS)
	tar -czvf $@ sysroot

.PHONY: floppy_boot.img
floppy_boot.img:
	dd if=/dev/zero of=$@ bs=512 count=1440
	mformat -C -f 1440 -i $@
	dd if=boot/bootsector.bin of=$@ conv=notrunc
	mcopy -i $@ boot/stpdboot.sys ::/STPDBOOT.SYS
	mcopy -i $@ kernel/vmstupid ::/VMSTUPID.SYS

.PHONY: clean
clean: $(SUBDIRS)
	$(RM) $(TARGET) $(SYSROOTDIR)
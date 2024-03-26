.EXPORT_ALL_VARIABLES:

TOPDIR     := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
SYSROOTDIR := $(TOPDIR)/sysroot
TOOLSDIR   := $(TOPDIR)/tools

BINDIR = /bin
LIBDIR = /usr/lib
INCDIR = /usr/include
ASMDIR = /usr/asm

AS = fasm
RM = rm -f

MK_BUGREPORT := \"https://git.cute.engineering/d0p1/StupidOS/issues\"
MK_COMMIT    := \"$(shell git rev-parse --short HEAD)\"

SUBDIRS	:= external tools include boot kernel lib bin

TARGET	= stupid.tar.gz floppy_boot.img
ifneq ($(OS),Windows_NT)
EXEXT	=
TARGET	+= stupid.iso stupid.hdd
else
EXEXT	= .exe
endif

.PHONY: all
all: $(TARGET)

GOAL:=install
clean: GOAL:=clean

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 $@"
	@DESTDIR=$(SYSROOTDIR) $(MAKE) -C $@ $(GOAL)

.PHONY: stupid.iso
stupid.iso: $(SUBDIRS)
	$(TOPDIR)/tools/create-iso $@ sysroot

.PHONY: stupid.hdd
stupid.hdd: $(SUBDIRS)
	@echo ""

.PHONY: stupid.tar.gz
stupid.tar.gz: $(SUBDIRS)
	tar -czvf $@ sysroot

.PHONY: floppy_boot.img
floppy_boot.img: $(SUBDIRS)
	dd if=/dev/zero of=$@ bs=512 count=1440
	mformat -C -f 1440 -i $@
	dd if=boot/bootsect/bootsector.bin of=$@ conv=notrunc
	mcopy -i $@ boot/loader/stpdldr.sys ::/STPDLDR.SYS
	mcopy -i $@ kernel/vmstupid.sys ::/VMSTUPID.SYS

.PHONY: run
run: all
	qemu-system-i386 \
		-rtc base=localtime \
		-drive file=floppy_boot.img,if=none,format=raw,id=boot \
		-drive file=fat:rw:./sysroot,if=none,id=hdd \
		-device floppy,drive=boot \
		-device ide-hd,drive=hdd \
		-global isa-fdc.bootindexA=0 \
		-serial mon:stdio

.PHONY: clean
clean: $(SUBDIRS)
	$(RM) $(TARGET) 

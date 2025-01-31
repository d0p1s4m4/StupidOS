.DEFAULT_GOAL:=all

export MAKEFLAGS += --no-print-directory

export LIB = 

export TOPDIR     := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
export SYSROOTDIR := $(TOPDIR)/sysroot
export TOOLSDIR   := $(TOPDIR)/tools

export BINDIR = /bin
export LIBDIR = /usr/lib
export INCDIR = /usr/include
export ASMDIR = /usr/asm

export AS = fasm
export CC ?= gcc
export LD = $(TOOLSDIR)/ld
export RM = rm -f
export TOOL_CC ?= gcc
export TOOL_LD ?= ld

export MK_BUGREPORT := \"https://git.cute.engineering/d0p1/StupidOS/issues\"
export MK_COMMIT    := \"$(shell git rev-parse --short HEAD)\"
export MK_PACKAGE   := \"StupidOS\"

export CFLAGS	= -DMK_COMMIT="$(MK_COMMIT)" \
			-DMK_BUGREPORT="$(MK_BUGREPORT)" \
			-DMK_PACKAGE="$(MK_PACKAGE)" \
			-I$(TOPDIR)include
export LDFLAGS	= 


QEMU_COMMON = \
		-rtc base=localtime \
		-vga cirrus \
		-serial stdio \
		-monitor telnet::4545,server,nowait \
		-net nic,model=ne2k_isa \
		-machine isapc 

SUBDIRS	:= external tools include boot kernel modules lib bin

TARGET	= stupid.tar.gz floppy1440.img floppy2880.img 
ifneq ($(OS),Windows_NT)
EXEXT	=
TARGET	+= stupid.iso stupid.hdd
else
EXEXT	= .exe
endif
export EXEXT

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
	dd if=/dev/zero of=$@ bs=1M count=0 seek=64
	mformat -L 12 -i $@
	mcopy -i $@ boot/loader/stpdldr.sys ::/STPDLDR.SYS
	mcopy -i $@ kernel/vmstupid.sys ::/VMSTUPID.SYS

.PHONY: stupid.tar.gz
stupid.tar.gz: $(SUBDIRS)
	tar -czvf $@ sysroot

.PHONY: floppy1440.img
floppy1440.img: $(SUBDIRS)
	dd if=/dev/zero of=$@ bs=512 count=1440
	mformat -C -f 1440 -i $@
	dd if=boot/bootsect/boot_floppy1440.bin of=$@ conv=notrunc
	mcopy -i $@ boot/loader/stpdldr.sys ::/STPDLDR.SYS
	mcopy -i $@ kernel/vmstupid.sys ::/VMSTUPID.SYS

.PHONY: floppy2880.img
floppy2880.img: $(SUBDIRS)
	dd if=/dev/zero of=$@ bs=512 count=2880
	mformat -C -f 2880 -i $@
	dd if=boot/bootsect/boot_floppy2880.bin of=$@ conv=notrunc
	mcopy -i $@ boot/loader/stpdldr.sys ::/STPDLDR.SYS
	mcopy -i $@ kernel/vmstupid.sys ::/VMSTUPID.SYS

OVMF32.fd:
	wget 'https://retrage.github.io/edk2-nightly/bin/DEBUGIa32_OVMF.fd' -O $@

.PHONY: run
run: all
	qemu-system-i386 \
		$(QEMU_COMMON) \
		-drive file=floppy1440.img,if=none,format=raw,id=boot \
		-drive file=fat:rw:./sysroot,if=none,id=hdd \
		-device floppy,drive=boot \
		-global isa-fdc.bootindexA=0

.PHONY: run-iso
run-iso: all
	qemu-system-i386 \
		$(QEMU_COMMON) \
		-cdrom stupid.iso

.PHONY: run-efi
run-efi: all OVMF32.fd
	qemu-system-i386 \
		-bios OVMF32.fd \
		-drive file=fat:rw:./sysroot,if=none,id=hdd \
		-device ide-hd,drive=hdd

.PHONY: bochs
bochs:
	bochs -f .bochsrc


.PHONY: docs
docs:
	@mkdir -p docs/html
	wget -O docs/webring.json https://webring.devse.wiki/webring.json
	python3 docs/devsewebring.py docs/webring.json docs/webring.txt
	naturaldocs -p docs/config -img docs/img -xi sysroot -i . -ro -o HTML docs/html
	cp docs/img/favicon.ico docs/html/

.PHONY: clean
clean: $(SUBDIRS)
	$(RM) $(TARGET) 

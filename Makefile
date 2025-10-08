.DEFAULT_GOAL:=all

export TOPDIR     := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
export BUILDDIR   := $(TOPDIR)/.build
export SYSROOTDIR := $(BUILDDIR)/sysroot
export TOOLSDIR   := $(BUILDDIR)/tools
export SYMSDIR    := $(BUILDDIR)/syms

export MAKEFLAGS += -I $(TOPDIR)/share/mk -Orecurse --no-print-directory

define subdir-rule

.PHONY: $1-$2
$1-$2:
	@echo "📁 $2"
	@DESTDIR=$(SYSROOTDIR) $$(MAKE) -C $2 $1

endef

SUBDIRS := external tools include lib bin sbin boot kernel modules tests

BUILD_RULES := build-tools .WAIT
BUILD_RULES += includes-kernel includes-include includes-lib .WAIT
BUILD_RULES += build-user build-kernel

USER_RULES := install-bin install-sbin
KERNEL_RULES := install-boot install-kernel install-modules

IMAGES = stupid.iso stupid.hdd floppy1440.img floppy2880.img

BUILD_START := $(shell date)

QEMU = qemu-system-i386
QEMU_COMMON = \
				-rtc base=localtime \
				-vga cirrus \
				-serial stdio \
				-monitor telnet::4545,server,nowait \
				-net nic,model=ne2k_isa \
				-machine isapc

.PHONY: all
all: $(IMAGES)
	@echo "build start: $(BUILD_START)"
	@printf "build end: " & date

.PHONY: build-tools
build-tools:
	@echo "📁 tools"
	@$(MAKE) -C tools install

.PHONY: build-user
build-user: $(USER_RULES)

.PHONY: build-kernel
build-kernel: $(KERNEL_RULES)

$(eval $(foreach X,kernel include lib,$(call subdir-rule,includes,$X)))
$(eval $(foreach X,bin sbin,$(call subdir-rule,install,$X)))
$(eval $(foreach X,boot kernel modules,$(call subdir-rule,install,$X)))

.PHONY: stupid.tar.gz
stupid.tar.gz: $(BUILD_RULES)
	tar -czvf $@ $(SYSROOTDIR)

# +------------------------------------------------------------------+
# | Disk images                                                      |
# +------------------------------------------------------------------+

.PHONY: stupid.iso
stupid.iso: $(BUILD_RULES)
	@echo "TODO"

.PHONY: stupid.hdd
stupid.hdd: $(BUILD_RULES)
	dd if=/dev/zero of=$@ bs=1M count=0 seek=64
	mformat -L 12 -i $@
	mcopy -i $@ boot/loader/stpdldr.sys "::/STPDLDR.SYS"
	mcopy -i $@ kernel/vmstupid.sys "::/VMSTUPID.SYS"

.PHONY: floppy1440.img
floppy1440.img: $(BUILD_RULES)
	dd if=/dev/zero of=$@ bs=512 count=1440
	mformat -C -f 1440 -i $@
	dd if=boot/bootsect/boot_floppy1440.bin of=$@ conv=notrunc
	mcopy -i $@ boot/loader/stpdldr.sys "::/STPDLDR.SYS"
	mcopy -i $@ kernel/vmstupid.sys "::/VMSTUPID.SYS"

.PHONY: floppy2880.img
floppy2880.img: $(BUILD_RULES)
	dd if=/dev/zero of=$@ bs=512 count=2880
	mformat -C -f 2880 -i $@
	dd if=boot/bootsect/boot_floppy2880.bin of=$@ conv=notrunc
	mcopy -i $@ boot/loader/stpdldr.sys "::/STPDLDR.SYS"
	mcopy -i $@ kernel/vmstupid.sys "::/VMSTUPID.SYS"

# +------------------------------------------------------------------+
# | QEMU / bochs                                                     |
# +------------------------------------------------------------------+

OVMF32.fd:
	wget 'https://retrage.github.io/edk2-nightly/bin/DEBUGIa32_OVMF.fd' -O $@

.PHONY: run
run: floppy1440.img
	$(QEMU) $(QEMU_COMMON) \
		-drive file=floppy1440.img,if=none,format=raw,id=boot \
		-drive file=flat:rw:$(SYSROOTDIR),if=none,id=hdd \
		-device floppy,drive=boot \
		-global isa-fdc.bootindexA=0

.PHONY: run-iso
run-iso: $(BUILD_RULES)
	$(QEMU) $(QEMU_COMMON) \
		-cdrom stupid.iso

.PHONY: run-efi
run-efi: OVMF32.fd $(BUILD_RULES)
	$(QEMU) -bios OVMF32.fd \
		-drive file=fat:rw:$(SYSROOTDIR),if=none,id=hdd \
		-device ide-hd,drive=hdd

.PHONY: bochs
bochs: $(BUILD_RULES)
	bochs -f .bochsrc

# +------------------------------------------------------------------+
# | Website / docs                                                   |
# +------------------------------------------------------------------+

.PHONY: docs
docs:
	@mkdir -p docs/html
	wget -O docs/webring.json "https://webring.devse.wiki/webring.json"
	python3 docs/devsewebring.py docs/webring.json docs/webring.txt
	naturaldocs -p docs/config -img docs/img -xi .build -i . -ro -o HTML docs/html
	cp docs/img/favicon.ico docs/html

# +------------------------------------------------------------------+
# | clean                                                            |
# +------------------------------------------------------------------+

$(eval $(foreach X,$(SUBDIRS),$(call subdir-rule,clean,$X)))

.PHONY: clean
clean: $(foreach X,$(SUBDIRS),clean-$X)
	rm -f $(IMAGES)

.PHONY: cleandir
cleandir: clean
	rm -rf $(BUILDDIR)

TOPDIR     := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
SYSROOTDIR := $(TOPDIR)/sysroot
TOOLSDIR   := $(TOPDIR)/tools

RM = echo

SUBDIRS	:= boot kernel lib bin

TARGET	= stupid.tar.gz
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

.PHONY: clean
clean: $(SUBDIRS)
	$(RM) $(TARGET) $(SYSROOTDIR)
# File: stupid.kernel.mk
# 
include $(TOPDIR)/share/mk/stupid.own.mk

ifdef KERNEL

KERNBASE ?= 0xC0000000

ASFLAGS	+= -D__KERNEL__ -I$(TOPDIR)/kernel -DKERNBASE=$(KERNBASE) \
			-I$(TOPDIR)/lib/base

OBJS = $(addsuffix .o, $(basename $(SRCS)))
OBJS-dbg  = $(addsuffix .dbg.o, $(basename $(SRCS)))

CLEANFILES	+= $(KERNEL) $(KERNEL)-dbg $(OBJS) $(OBJS-dbg)

$(KERNEL): $(OBJS)
	$(LD) -T $(TOPDIR)/kernel/linker.ld -nostdlib -o $@ $^

$(KERNEL)-dbg: $(OBJS-dbg)
	$(LD) -T $(TOPDIR)/kernel/linker.ld -nostdlib -o $@ $^

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

%.dbg.o: %.s
	$(AS) $(ASFLAGS) $(ASDBGFLAGS) -o $@ $<

lib/%.o: ../lib/base/%.s
	@$(MKCWD)
	$(AS) $(ASFLAGS) -o $@ $<

lib/%.dbg.o: ../lib/base/%.s
	@$(MKCWD)
	$(AS) $(ASFLAGS) $(ASDBGFLAGS) -o $@ $<

all: $(KERNEL) $(KERNEL)-dbg

install:: $(KERNEL) $(KERNEL)-dbg
	$(INSTALL) -d $(DESTDIR)/
	$(INSTALL) $^ $(DESTDIR)/

endif

include $(TOPDIR)/share/mk/stupid.clean.mk

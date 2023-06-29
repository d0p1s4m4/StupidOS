# File: stupid.kernel.mk
# 
include $(TOPDIR)/share/mk/stupid.own.mk

ifdef KERNEL

ASFLAGS	+= -D__KERNEL__ -I$(TOPDIR)/kernel

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

endif

include $(TOPDIR)/share/mk/stupid.clean.mk

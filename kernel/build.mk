include kernel/drivers/build.mk

KERNEL_SRCS	= head.s gdt.s pic.s isr.s idt.s pmm.s paging.s lib/log.s
KERNEL_OBJS	= $(addprefix kernel/, $(KERNEL_SRCS:.s=.o) $(DRIVERS_OBJS))
KERNEL_BIN	= vmstupid
KERNEL_DUMP = $(KERNEL_BIN).dump

KERNEL_ASFLAGS	= $(ASFLAGS) -D__KERNEL__ -Ikernel

GARBADGE += $(KERNEL_OBJS) $(KERNEL_BIN) $(KERNEL_DUMP)

$(KERNEL_BIN): $(KERNEL_OBJS)
	$(LD) -T $(KERNEL_DIR)/linker.ld -nostdlib -o $@ $^

$(KERNEL_DUMP): $(KERNEL_BIN)
	$(OBJDUMP) -D $^ > $@

kernel/lib/%.o: lib/base/%.s
	@$(MKCWD)
	$(AS) -felf -o $@ $< $(KERNEL_ASFLAGS)

kernel/%.o: kernel/%.s
	$(AS) -felf -o $@ $< $(KERNEL_ASFLAGS)


include kernel/drivers/build.mk

KERNEL_SRCS	= head.s gdt.s idt.s paging.s lib/log.s
KERNEL_OBJS	= $(addprefix kernel/, $(KERNEL_SRCS:.s=.o) $(DRIVERS_OBJS))
KERNEL_BIN	= stupid.elf
KERNEL_DUMP = $(KERNEL_BIN:.elf=.dump)

KERNEL_ASFLAGS	= $(ASFLAGS) -D__KERNEL__

GARBADGE += $(KERNEL_OBJS) $(KERNEL_BIN) $(KERNEL_DUMP)

$(KERNEL_BIN): $(KERNEL_OBJS)
	$(LD) -T $(KERNEL_DIR)/linker.ld -nostdlib -o $@ $^

kernel/lib/%.o: lib/base/%.s
	mkdir -p $(@D)
	$(AS) -felf -o $@ $< $(KERNEL_ASFLAGS)

kernel/%.o: kernel/%.s
	$(AS) -felf -o $@ $< $(KERNEL_ASFLAGS)


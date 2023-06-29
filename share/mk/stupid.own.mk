# File: stupid.own.mk
# Define common variables
ifndef _STUPID_OWN_MK_
_STUPID_OWN_MK_=1

.DEFAULT_GOAL := all

BINDIR	= /bin
LIBDIR	= /lib

CC		= clang -target i386-none-elf
CXX		= clang++ -target i386-none-elf

AS		= nasm
LD		= ld.lld
PLSCC	= plsc
OBJCOPY	= llvm-objcopy
OBJDUMP = llvm-objdump

MKCWD	= mkdir -p $(@D)

TARGETS	= all clean install
.PHONY: $(TARGETS)

EXTRAFLAGS	= -DSTUPID_VERSION="0.0" -D__STUPID__

CFLAGS		+= -Wall -Werror -Wextra $(EXTRAFLAGS)
CXXFLAGS	+= -Wall -Werror -Wextra $(EXTRAFLAGS)
ASFLAGS		+= -felf -I$(TOPDIR)/lib $(EXTRAFLAGS)
ASDBGFLAGS	+=  -F dwarf -g -DDEBUG

endif

LIBS_BIN	=

define LIBS_TEMPLATE

$(1)_BIN = lib$(1).a

$(1)_OBJS = $$(addprefix lib/$(1)/, $$($(1)_SRCS:.s=.o))

$$($(1)_BIN): $$($(1)_OBJS)
	ar rcs $$@ $$^

GARBADGE	+= $$($(1)_OBJS) $$($(1)_BIN)
LIBS_BIN	+= $$($(1)_BIN)

endef

LIBS	=

include lib/*/build.mk

$(foreach lib, $(LIBS), $(eval $(call LIBS_TEMPLATE,$(lib))))

lib/%.o: lib/%.s
	$(AS) $(ASFLAGS) -o $@ $<

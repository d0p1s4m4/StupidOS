include stpd.base.mk

define includes-rule

$1/$2: $2
	$$(MSG_INSTALL)
	@install -D $2 $1/$2

endef

_INCLUDES:=$(addprefix $(DESTDIR)$(INCDIR)/, $(INCS)) \
			$(addprefix $(DESTDIR)$(ASMDIR)/, $(ASMINCS))

includes: $(_INCLUDES)

$(eval $(foreach X,$(INCS),$(call includes-rule,$(DESTDIR)$(INCDIR),$X)))
$(eval $(foreach X,$(ASMINCS),$(call includes-rule,$(DESTDIR)$(ASMDIR),$X)))

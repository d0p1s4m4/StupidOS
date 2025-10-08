include stpd.base.mk

ifdef SUBDIRS

_SUBDIRS=$(filter-out .WAIT,$(SUBDIRS))
_CURDIR=$(subst $(TOPDIR),,$(CURDIR))
ifneq ($(_CURDIR),)
_CURDIR:=$(_CURDIR)/
endif

define subdir-rule

.PHONY: $1-$2
$1-$2:
	@echo "📁 $(_CURDIR)$2"
	@DESTDIR=$(DESTDIR) $$(MAKE) -C $2 $1

endef

define subdir-target

.PHONY: subdir-$1
subdir-$1: $(foreach X,$(SUBDIRS),$(if $(filter-out $(X),.WAIT),$1-$X,.WAIT))

$1: subdir-$1

endef

$(eval $(foreach T,$(TARGETS),$(foreach X,$(_SUBDIRS),$(call subdir-rule,$T,$X))))
$(eval $(foreach T,$(TARGETS),$(call subdir-target,$T)))

endif


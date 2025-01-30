TARGETS = all install clean
.PHONY: $(TARGETS)
$(TARGETS): ;

_INST =  $(addprefix $(DESTDIR)$(INCDIR)/, $(INCS)) \
		$(addprefix $(DESTDIR)$(ASMDIR)/, $(ASMINCS)) \
		$(addprefix $(DESTDIR)$(LIBDIR)/, $(LIB)) 

# ----------------- Subdirs -----------------------

ifdef SUBDIRS

_SUBDIRS:=$(filter-out .WAIT,$(SUBDIRS))
_CURDIR=$(subst $(TOPDIR),,$(CURDIR))
ifneq ($(_CURDIR),)
_CURDIR:=$(_CURDIR)/
endif

.PHONY: $(_SUBDIRS)
$(_SUBDIRS):
	@echo "📁 $(_CURDIR)$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

$(TARGETS): $(SUBDIRS)

endif

# --------------- Build rules ------------------

%.o: %.asm
	$(AS) $< $@

# --------------- Lib rules --------------------

ifneq ($(LIB),)

all: $(LIB)

_cleanlib:
	$(RM) $(LIB)

clean: _cleanlib

endif

# --------------- Progs rules -------------------

ifneq ($(PROG),)

all: $(PROG)


endif

# --------------- Install rules -----------------

ifneq ($(_INST),)

install: $(_INST)

endif

# headers
$(DESTDIR)$(INCDIR)/%.h: %.h
	install -D $< $@

$(DESTDIR)$(ASMDIR)/%.inc: %.inc
	install -D $< $@

# libs
$(DESTDIR)$(LIBDIR)/%.a: %.a
	install -D $< $@

$(DESTDIR)$(LIBDIR)/%.o: %.o
	install -D $< $@

# bins
$(DESTDIR)$(BINDIR)/%: %
	install -D $< $@

# kernel & mods
$(DESTDIR)/%.sys: %.sys
	install -D $< $@

$(DESTDIR)/%.mod: %.mod
	install -D $< $@


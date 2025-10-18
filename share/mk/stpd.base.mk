ifndef _STPD_BASE_MK_
_STPD_BASE_MK_:=1

ifndef TOPDIR
$(error "TOPDIR is undefined")
endif

MK_BUGREPORT := \"https://git.cute.engineering/d0p1/StupidOS/issues\"
MK_COMMIT    := \"$(shell git rev-parse --short HEAD)\"
MK_PACKAGE   := \"StupidOS\"

BASE_CPPFLAGS = -DMK_COMMIT="$(MK_COMMIT)" \
			-DMK_BUGREPORT="$(MK_BUGREPORT)" \
			-DMK_PACKAGE="$(MK_PACKAGE)"

TARGETS = all clean includes install

BINDIR ?= /bin
LIBDIR ?= /usr/lib
INCDIR ?= /usr/include
ASMDIR ?= /usr/asm
MANDIR ?= /usr/share/man
DOCDIR ?= /usr/share/doc

# +------------------------------------------------------------------+
# | Tools                                                            |
# +------------------------------------------------------------------+
ifndef TOOLSDIR
$(error "TOOLSDIR is undefined")
endif

FASM    = INCLUDE=$(SYSROOTDIR)$(ASMDIR) fasm
CC      = $(TOOLSDIR)/bin/tcc
AR      = ar
FAS2SYM = $(TOOLSDIR)/bin/fas2sym

define VERSION_SH =
$(MSG_CREATE)
@sh $(TOPDIR)/tools/version.sh $< $@
endef

# +------------------------------------------------------------------+
# | Host                                                             |
# +------------------------------------------------------------------+
HOST_CC       = gcc
HOST_AR       = ar
HOST_LDFLAGS  += -L$(TOOLSDIR)/lib
HOST_CFLAGS   +=
HOST_CPPFLAGS += $(BASE_CPPFLAGS) -I $(TOPDIR)/include

# +------------------------------------------------------------------+
# | Verbose                                                          |
# +------------------------------------------------------------------+
MSG         ?= @echo '  '
MSG_BUILD   ?= $(MSG) '👷‍♂️    build ' $@
MSG_CREATE  ?= $(MSG) '🪄   create ' $@
MSG_COMPILE ?= $(MSG) '⚙️  compile ' $@
MSG_LINK    ?= $(MSG) '🔗     link ' $@
MSG_EXECUTE ?= $(MSG) '▶️  execute ' $@
MSG_FORMAT  ?= $(MSG) '✏️   format ' $@
MSG_INSTALL ?= $(MSG) '📦  install ' $(subst $(TOPDIR)/,,$@)
MSG_REMOVE  ?= $(MSG) '🗑️   remove ' $@

# +------------------------------------------------------------------+
# | rules                                                            |
# +------------------------------------------------------------------+

%.o: %.asm
	$(MSG_COMPILE)
	@$(FASM) $< $@ >/dev/null

%.lo: %.c
	$(MSG_COMPILE)
	@$(HOST_CC) -c -o $@ $< $(HOST_CPPFLAGS) $(HOST_CFLAGS)


$(TARGETS):

endif

include stpd.base.mk

all: crt0.o

install: $(DESTDIR)$(LIBDIR)/crt0.o

$(DESTDIR)$(LIBDIR)/crt0.o: crt0.o
	$(MSG_INSTALL)
	@install -D $< $@

CLEANFILES = crt0.o

include stpd.clean.mk

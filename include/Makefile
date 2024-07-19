INCS = coff.h elf.h
SYSINCS = sys/errno.h
INCSYSDIR = $(INCDIR)/sys

.PHONY: all
all:

.PHONY: clean
clean:

.PHONY: install
install: $(INCS) $(SYSINCS)
	@ mkdir -p $(DESTDIR)$(INCDIR)
	install $(INCS) $(DESTDIR)$(INCDIR)
	@ mkdir -p $(DESTDIR)$(INCSYSDIR)
	install $(SYSINCS) $(DESTDIR)$(INCSYSDIR)

.PHONY: all clean install

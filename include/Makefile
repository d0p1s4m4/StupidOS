INCDIR = $(DESTDIR)/usr/include

INCS = coff.h

.PHONY: all
all:

.PHONY: clean
clean:

.PHONY: install
install: $(INCS)
	@ mkdir -p $(INCDIR)
	install $< $(INCDIR)

.PHONY: all clean install

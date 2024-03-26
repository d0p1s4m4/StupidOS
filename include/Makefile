INCS = coff.h

.PHONY: all
all:

.PHONY: clean
clean:

.PHONY: install
install: $(INCS)
	@ mkdir -p $(DESTDIR)$(INCDIR)
	install $< $(DESTDIR)$(INCDIR)

.PHONY: all clean install

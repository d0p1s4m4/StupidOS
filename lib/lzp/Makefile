LIBS	= liblzp.a
OBJS	= lzp.o

INCS = lzp.h
ASMINCS = lzp.inc

INST = $(addprefix $(DESTDIR)$(INCDIR)/, $(INCS)) \
		$(addprefix $(DESTDIR)$(ASMDIR)/, $(ASMINCS)) \
		$(addprefix $(DESTDIR)$(LIBDIR)/, $(LIBS))

.PHONY: all
all: $(LIBS)

$(LIBS): $(OBJS)
	$(AR) rcs $@ $^

%.o: %.asm
	$(AS) $< $@

$(DESTDIR)$(INCDIR)/%.h: %.h
	install -D $< $@

$(DESTDIR)$(ASMDIR)/%.inc: %.inc
	install -D $< $@

$(DESTDIR)$(LIBDIR)/%.a: %.a
	install -D $< $@

.PHONY: clean
clean:
	$(RM) $(LIBS) $(OBJS)

.PHONY: install
install: $(INST)


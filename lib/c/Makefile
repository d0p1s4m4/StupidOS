LIBS	= libc.a
OBJS	= ctype.o
INCS	= stddef.h time.h ctype.h

INST = $(addprefix $(DESTDIR)$(INCDIR)/, $(INCS)) \
		$(addprefix $(DESTDIR)$(LIBDIR)/, $(LIBS))

.PHONY: all
all: $(LIBS)

$(LIBS): $(OBJS)
	$(AR) rcs $@ $^

%.o: %.asm
	$(AS) $< $@

$(DESTDIR)$(INCDIR)/%.h: %.h
	install -D $< $@

$(DESTDIR)$(LIBDIR)/%.a: %.a
	install -D $< $@

.PHONY: clean
clean:
	$(RM) $(LIBS) $(OBJS)

.PHONY: install
install: $(INST)

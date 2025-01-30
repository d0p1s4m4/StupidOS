LIBS	= crt0.o

INST	= $(addprefix $(DESTDIR)$(LIBDIR)/, $(LIBS))

all: $(LIBS)

%.o: %.asm
	$(AS) $< $@

$(DESTDIR)$(LIBDIR)/%.o: %.o
	install -D $< $@

clean:
	$(RM) $(LIBS)

install: $(INST)

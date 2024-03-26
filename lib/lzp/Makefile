TARGET	= liblzp.a
OBJS	= lzp.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

%.o: %.asm
	$(AS) $< $@

clean:
	$(RM) $(TARGET) $(OBJS)

install: $(TARGET)
	@ mkdir -p $(DESTDIR)$(LIBDIR)
	install $(TARGET) $(DESTDIR)$(LIBDIR)
	@ mkdir -p $(DESTDIR)$(INCDIR)
	install lzp.h $(DESTDIR)$(INCDIR)
	@ mkdir -p $(DESTDIR)$(ASMDIR)
	install lzp.inc $(DESTDIR)$(ASMDIR)

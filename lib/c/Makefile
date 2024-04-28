TARGET	= libc.a
OBJS	= ctype.o
INCS	= stddef.h time.h ctype.h

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
	install $(INCS) $(DESTDIR)$(INCDIR)

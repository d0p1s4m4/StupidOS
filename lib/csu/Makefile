TARGET	= crt0.o

all: $(TARGET)

%.o: %.asm
	$(AS) $< $@

clean:
	$(RM) $(TARGET)

install: $(TARGET)
	@ mkdir -p $(DESTDIR)
	install $(TARGET) $(DESTDIR)

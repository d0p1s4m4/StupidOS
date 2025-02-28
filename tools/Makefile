TARGET	= ld$(EXEXT) parted$(EXEXT) readcoff$(EXEXT) elf2coff$(EXEXT) ar$(EXEXT) ranlib$(EXEXT)

.PHONY: all
all: $(TARGET)

ld$(EXEXT): ../bin/ld/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

readcoff$(EXEXT): ../bin/readcoff/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

elf2coff$(EXEXT): ../bin/elf2coff/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

parted$(EXEXT): ../sbin/parted/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

ranlib$(EXEXT): ../bin/ranlib/main.c ../bin/ar/archive.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

ar$(EXEXT): ../bin/ar/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: install
install: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET)

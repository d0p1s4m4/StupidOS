TARGET	= fat$(EXEXT) coff-ld$(EXEXT) parted$(EXEXT) readcoff$(EXEXT)

.PHONY: all
all: $(TARGET)

fat$(EXEXT): fat.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

coff-ld$(EXEXT): ../bin/ld/main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

readcoff$(EXEXT): ../bin/readcoff/main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

parted$(EXEXT): ../sbin/parted/main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: install
install: $(TARGET)

.PHONY: clean
clean:

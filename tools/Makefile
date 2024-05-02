TARGET	= fat$(EXEXT) coff-ld$(EXEXT) parted$(EXEXT) mkfs.stpd$(EXEXT)

.PHONY: all
all: $(TARGET)

fat$(EXEXT): fat.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

coff-ld$(EXEXT): coff-ld.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

parted$(EXEXT): ../sbin/parted/main.c
ifneq (,$(findstring cl,$(CC)))
	$(CC) /Fo:$@ $^  $(CFLAGS) $(LDFLAGS)
else
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
endif

mkfs.stpd$(EXEXT): ../sbin/mkfs.stpd/main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)	

.PHONY: install
install: $(TARGET)

.PHONY: clean
clean:

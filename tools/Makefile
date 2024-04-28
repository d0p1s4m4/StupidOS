CC	= gcc
RM	= rm -f

TARGET	= fat$(EXEXT) coff-ld$(EXEXT) parted$(EXEXT) mkfs.stpd$(EXEXT)

CFLAGS	= -DMK_COMMIT="$(MK_COMMIT)" -DMK_BUGREPORT="$(MK_BUGREPORT)" -I../include
LDFLAGS	=

.PHONY: all
all: $(TARGET)

fat$(EXEXT): fat.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

coff-ld$(EXEXT): coff-ld.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

parted$(EXEXT): ../sbin/parted/main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

mkfs.stpd$(EXEXT): ../sbin/mkfs.stpd/main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)	

.PHONY: install
install: $(TARGET)

.PHONY: clean
clean:

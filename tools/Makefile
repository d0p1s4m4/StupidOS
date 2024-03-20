CC	= gcc
RM	= rm -f

TARGET	= fat$(EXEXT) coff-ld$(EXEXT)

CFLAGS	= -DMK_COMMIT="$(MK_COMMIT)" -DMK_BUGREPORT="$(MK_BUGREPORT)" -I../include
LDFLAGS	=

.PHONY: all
all: $(TARGET)

fat$(EXEXT): fat.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

coff-ld$(EXEXT): coff-ld.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: install
install: $(TARGET)

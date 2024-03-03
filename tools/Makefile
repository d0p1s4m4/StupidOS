CC	= gcc
RM	= rm -f

TARGET	= fat$(EXEXT)

CFLAGS	= -DMK_COMMIT="$(MK_COMMIT)" -DMK_BUGREPORT="$(MK_BUGREPORT)"
LDFLAGS	=

.PHONY: all
all: $(TARGET)

fat$(EXEXT): fat.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: install
install: $(TARGET)

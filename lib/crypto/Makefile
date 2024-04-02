TARGET	= libcrypto.a
OBJS	= sha2/sha256.o sha2/sha512.o \
			chacha20/chacha20.o
INCS	= sha2/sha2.h \
			chacha20/chacha20.h \
			dilithium/dilithium.h \
			falcon/falcon.h \
			keccak/keccak.h
ASMS	= sha2/sha2.inc \
			chacha20/chacha20.inc

INCCRYPOTDIR = $(INCDIR)/crypto
ASMCRYPTODIR = $(ASMDIR)/crypto

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
	@ mkdir -p $(DESTDIR)$(INCCRYPOTDIR)
	install $(INCS) $(DESTDIR)$(INCCRYPOTDIR)
	install crypto.h $(DESTDIR)$(INCDIR)
	@ mkdir -p $(DESTDIR)$(ASMCRYPTODIR)
	install $(ASMS) $(DESTDIR)$(ASMCRYPTODIR)



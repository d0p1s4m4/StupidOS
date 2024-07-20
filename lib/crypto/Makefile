TARGET	= libcrypto.a
OBJS	= sha2/sha256.o sha2/sha512.o \
			chacha/chacha.o \
			hchacha/hchacha.o
INCS	= sha2/sha2.h \
			chacha/chacha.h \
			hchacha/hchacha.h \
			dilithium/dilithium.h \
			falcon/falcon.h \
			keccak/keccak.h
ASMS	= sha2/sha2.inc \
			chacha/chacha.inc \
			hchacha/hchacha.inc

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



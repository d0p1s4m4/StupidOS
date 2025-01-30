LIBS	= libcrypto.a
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

INST = $(addprefix $(DESTDIR)$(INCDIR)/, $(INCS)) \
		$(addprefix $(DESTDIR)$(ASMDIR)/, $(ASMS)) \
		$(addprefix $(DESTDIR)$(LIBDIR)/, $(LIBS))

.PHONY: all
all: $(LIBS)

$(LIBS): $(OBJS)
	$(AR) rcs $@ $^

%.o: %.asm
	$(AS) $< $@

$(DESTDIR)$(INCDIR)/%.h: %.h
	install -D $< $@

$(DESTDIR)$(ASMDIR)/%.inc: %.inc
	install -D $< $@

$(DESTDIR)$(LIBDIR)/%.a: %.a
	install -D $< $@

.PHONY: clean
clean:
	$(RM) $(LIBS) $(OBJS)

.PHONY: install
install: $(INST)



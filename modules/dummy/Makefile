MODULE = dummy.mod

INST = $(addprefix $(DESTDIR)/, $(MODULE))

all:

clean:

$(MODULE): $(MODULE:.mod=.asm)
	fasm $^ $@

$(DESTDIR)/%.mod: %.mod
	install -D $< $@

.PHONY: install
install: $(INST)

MODULE = dummy.mod

all:

clean:

$(MODULE): $(MODULE:.mod=.asm)
	fasm $^ $@

.PHONY: install
install: $(MODULE)
	@ mkdir -p $(DESTDIR)
	install $< $(DESTDIR)

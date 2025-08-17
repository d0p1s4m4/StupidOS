TARGET	= ld$(EXEXT) parted$(EXEXT) readcoff$(EXEXT) \
			elf2coff$(EXEXT) ar$(EXEXT) ranlib$(EXEXT) \
			fasdump$(EXEXT) fas2sym$(EXEXT)

FAS2SYM_SRCS = main.c fas.c elf.c buffer.c
FAS2SYM_OBJS = $(addprefix ../bin/fas2sym/, $(FAS2SYM_SRCS:.c=.o))

.PHONY: all
all: $(TARGET)

ld$(EXEXT): ../bin/ld/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

readcoff$(EXEXT): ../bin/readcoff/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

elf2coff$(EXEXT): ../bin/elf2coff/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

parted$(EXEXT): ../sbin/parted/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

ranlib$(EXEXT): ../bin/ranlib/main.c ../bin/ar/archive.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

ar$(EXEXT): ../bin/ar/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

fasdump$(EXEXT): ../bin/fasdump/main.c
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

fas2sym$(EXEXT): $(FAS2SYM_OBJS)
	$(TOOL_CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: install
install: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET)

TOPGOALS = all clean install

SUBDIRS = bootsect loader efi

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 boot/$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

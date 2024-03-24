TOPGOALS = all clean install

SUBDIRS = bootsect loader efi

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 boot/$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

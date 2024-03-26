SUBDIRS = csu crypto lzp

TOPGOALS = all clean install

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 lib/$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

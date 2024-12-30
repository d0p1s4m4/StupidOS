SUBDIRS = csu crypto lzp c

TOPGOALS = all clean install

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 lib/$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

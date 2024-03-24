SUBDIRS = csu crypto

TOPGOALS = all clean install

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 lib/$@"
	@DESTDIR=$(DESTDIR)/usr/lib $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

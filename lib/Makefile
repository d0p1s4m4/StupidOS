SUBDIRS = crypto

TOPGOALS = all clean install

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 lib/$@"
	DESTDIR=$(DESTDIR)/lib $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

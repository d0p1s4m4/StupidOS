TOPGOALS = all clean install

SUBDIRS = dummy

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 modules/$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

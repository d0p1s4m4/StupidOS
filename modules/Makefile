TOPGOALS = all clean install

SUBDIRS = dummy

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 modules/$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

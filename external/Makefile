TOPGOALS = all clean install

SUBDIRS =

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 external/$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

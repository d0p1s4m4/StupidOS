TOPGOALS = all clean install

SUBDIRS =

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 external/$@"
	@DESTDIR=$(DESTDIR) $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

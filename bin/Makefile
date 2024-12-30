TOPGOALS = all clean install

SUBDIRS = cmd

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 bin/$@"
	@DESTDIR=$(DESTDIR)/bin $(MAKE) -C $@ $(MAKECMDGOALS)

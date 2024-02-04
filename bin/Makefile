SUBDIRS = cmd

TOPGOALS = all clean install

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 bin/$@"
	DESTDIR=$(DESTDIR)/bin $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

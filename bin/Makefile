SUBDIRS = cmd

TOPGOALS = all clean install

SUBDIRS = cmd

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "📁 bin/$@"
	DESTDIR=$(DESTDIR)/bin $(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPGOALS)
$(TOPGOALS): $(SUBDIRS)

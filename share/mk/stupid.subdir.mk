# File: stupid.subdir.mk
# Targets for building subdirectories

.DEFAULT_GOAL := all
TOPGOALS = all clean install

ifndef NOSUBDIR

.PHONY: $(SUBDIR)
$(SUBDIR):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(TOPGOALS): $(SUBDIR)

else

# ensure target exist
$(TOPGOALS):

endif

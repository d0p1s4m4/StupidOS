ifndef _STPD_CLEAN_MK_
_STPD_CLEAN_MK_=1

.PHONY: clean
clean:
ifeq ($(strip $(CLEANFILES)),)
	@true
else
	rm -f $(CLEANFILES)
endif

endif

define TOOLS_TEMPLATE

$(1) = $$(addprefix $$(TOOLS_DIR)/, $$(shell echo -n $(1) | tr A-Z a-z))

endef

TOOLS = GIT-VERSION CREATE-ISO

$(foreach tool, $(TOOLS), $(eval $(call TOOLS_TEMPLATE,$(tool))))

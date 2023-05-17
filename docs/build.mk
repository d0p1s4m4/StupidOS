.PHONY: docs
docs: 
	-mkdir -p $(DOC_DIR)/html
	naturaldocs -p $(DOC_DIR)/config -i . -o HTML $(DOC_DIR)/html

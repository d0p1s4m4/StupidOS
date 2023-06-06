.PHONY: docs
docs: 
	-mkdir -p $(DOC_DIR)/html
	naturaldocs -p $(DOC_DIR)/config -xi tmp -i . -o HTML $(DOC_DIR)/html
	cp .github/favicon.ico $(DOC_DIR)/html/

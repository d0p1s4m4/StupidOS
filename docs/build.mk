.PHONY: docs
docs: 
	-mkdir -p $(DOC_DIR)/html
	naturaldocs -p $(DOC_DIR)/config -img $(DOC_DIR)/img -xi tmp -i . -o HTML $(DOC_DIR)/html
	cp $(DOC_DIR)/img/favicon.ico $(DOC_DIR)/html/

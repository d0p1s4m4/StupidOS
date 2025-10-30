	;; File: cfg.inc
	;;
	;; Basic boot.cfg file
	;; >
	;; > textmode=no
	;; > resolution=1024x768x32
	;; >

	;; Define: CFG_COMMENT
CFG_COMMENT        = 0x3B		; ';'
	;; Define: CFG_ENTRY_SEP
CFG_ENTRY_SEP      = 0x3D		; '='
	;; Define: CFG_RESOLUTION_SEP
CFG_RESOLUTION_SEP = 0x78		; 'x'

	;; Function: boot_cfg_parse
	;;
boot_cfg_parse:
	ret

	;; Function: cfg_textmode_handler
cfg_textmode_handler:
	ret

	;; Function: cfg_resolution_handler
cfg_resolution_handler:
	ret

	;; Function: cfg_cmdline_handler
cfg_cmdline_handler:
	ret

	;; Function: cfg_kernel_handler
cfg_kernel_handler:
	ret

	;; Variable: bCfgTextMode
bCfgTextMode    db TRUE
	;; Variable: uCfgVideoWidth
uCfgVideoWidth  dw 1024
	;; Variable: uCfgVideoHeight
uCfgVideoHeight dw 768
	;; Variable: uCfgVideoDepth
uCfgVideoDepth  db 32

	;; Constant: szCfgFile
szCfgFile          db "BOOT    CFG", 0
	;; Constant: szErrorCfgNotFound
szErrorCfgNotFound db "boot.cfg not found", CR, LF, 0
	;; Constant: szCfgYes
szCfgYes           db "yes", 0
	;; Constant: szCfgNo
szCfgNo            db "no", 0
CfgTable:
	db 8, "textmode"
	dw cfg_textmode_handler
	db 10, "resolution"
	dw cfg_resolution_handler
	db 7, "cmdline"
	dw cfg_cmdline_handler
	db 6, "kernel"
	dw cfg_kernel_handler

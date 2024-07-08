	format COFF
	use32

	include '../module.inc'

	section '.text' code

	public module_init
module_init:
	ret

	public module_exit
module_exit:
	ret

MODULE_INFO_BEGIN

MODULE_AUTHOR "d0p1"
MODULE_DESCRIPTION "Dummy module"
MODULE_LICENSE "BSD-3-Clause"
MODULE_VERSION "1.0"

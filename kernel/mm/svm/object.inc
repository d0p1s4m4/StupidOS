	;; File: object.inc

struc SvmObject {
	.refcount dd ?

	.amap dd ?
	.object dd ?
	
	.ops dd ?
}

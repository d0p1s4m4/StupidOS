	;; File: object.inc
SVM_OBJ_ANON   = 1
SVM_OBJ_FILE   = 2
SVM_OBJ_DEVICE = 3


struc SvmObject {
	.refcount dd ?

	.amap dd ?
	.object dd ?
	
	.ops dd ?
}

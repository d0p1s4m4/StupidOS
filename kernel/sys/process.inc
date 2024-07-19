struc Context {
	.edi dd ?
	.esi dd ?
	.ebx dd ?
	.ebp dd ?
	.eip dd ?
}
DEFN Context

struc Process {
	.pagedir   dd ?
	.kstack    dd ?
	.parent    dd ?
	.trapframe dd ?
	.context   dd ?
	.next      dd ?
}
DEFN Process

PROCESS_STATE_IDL    = 0
PROCESS_STATE_RUN    = 1
PROCESS_STATE_SLEEP  = 2
PROCESS_STATE_STOP   = 3
PROCESS_STATE_ZOMBIE = 4

PZERO  = 22
PWAIT  = 24
PLOCK  = 36
PPAUSE = 40
PUSER  = 50

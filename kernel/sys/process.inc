struc Context {
	.gs  dw ?
	.fs  dw ?
	.es  dw ?
	.ds  dw ?
	.ss  dw ?

	.eax dd ?
	.ebx dd ?
	.ecx dd ?
	.edx dd ?
	.esi dd ?
	.edi dd ?
	.ebp dd ?

	.error  dd ?
	.eip    dd ?
	.cs     dd ?
	.eflags dd ?
}
DEFN Context

struc Process {
	.state     dd ?
	.pagedir   dd ?
	.kstack    dd ?
	.parent    dd ?
	.trapframe dd ?
	.context   dd ?
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

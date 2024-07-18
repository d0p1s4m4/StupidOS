
struc ProcessGroup {
}

struc Context {
	.flags dd ?
	.regs  dd ?
}

struc Process {
	.pagedir dd ?
	.parent  dd ?
	.context Context
	.next    dd ?
}

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

	;; File: mbr.inc


struc Partition
{
	.status    db ?
	.chs_start db 3 dup(?)
	.type      db ?
	.chs_last  db 3 dup(?)
	.lba       dd ?
	.sectors   dd ?
}
DEFN Partition



#include <fdisk/disk.h>

static int
msdos_probe(struct disk *dsk)
{
	(void)dsk;
	return (0);
}

DEF_DISK_OPS(msdos);
DEF_DISK_LABEL(msdos);

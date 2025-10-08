#include <fdisk/disk.h>

int
disk_init(struct disk *dsk, struct device *dev)
{
	if (dsk == NULL || dev == NULL)
	{
		return (-1);
	}

	memset(dsk, 0, sizeof(struct disk));
	disk->dev = dev;
}

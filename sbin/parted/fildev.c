#include <stdio.h>
#include <fdisk/device.h>

#define DEFAULT_SECSZ 512

static FILE *fildev_fp = NULL;

int
device_open(struct device *dev, const char *path)
{
	long sz;

	if (dev == NULL || path == NULL)
	{
		return (-1);
	}

	dev->path = path;
	fildev_fp = fopen(dev->path, "r+b");
	if (fildev_fp == NULL)
	{
		/* XXX: error msg */
		return (-1);
	}

	if (fseek(fildev_fp, 0, SEEK_END) != 0)
	{
		goto err;
	}

	sz = ftell(fildev_fp);
	if (sz < 0)
	{
		goto err;
	}

	dev->size = (uint64_t)sz;
	dev->ph_secsz = DEFAULT_SECSZ;
	dev->ph_secnt = dev->size / dev->ph_secsz;

	dev->status = DEV_OPEN;

	return (0);
err:
	fclose(fildev_fp);
	return (-1);
}

int
device_close(struct device *dev)
{
	if (dev == NULL || fildev_fp == NULL)
	{
		return (-1);
	}

	if (DEVICE_IS_OPEN(dev))
	{
		fclose(fildev_fp);
		fildev_fp = NULL;
		dev->status &= ~(DEV_OPEN);
	}

	return (0);
}

int
device_read(struct device *dev, uint8_t *buff, uint64_t lba,
			int count)
{
	if (dev == NULL || buff == NULL || count == 0)
	{
		return (-1);
	}

	if (!DEVICE_IS_OPEN(dev))
	{
		return (-1);
	}

	if (fseek(fildev_fp, dev->ph_secsz * lba, SEEK_SET) != 0)
	{
		return (-1);
	}

	if (fread(buff, dev->ph_secsz, count, fildev_fp) != count)
	{
		return (-1);
	}

	return (0);
}

int
device_write(struct device *dev, const uint8_t *buff, uint64_t lba,
			 int count)
{
	if (dev == NULL || fildev_fp == NULL || buff == NULL || count == 0)
	{
		return (-1);
	}

	if (!DEVICE_IS_OPEN(dev))
	{
		return (-1);
	}

	if (fseek(fildev_fp, dev->ph_secsz * lba, SEEK_SET) != 0)
	{
		return (-1);
	}

	if (fwrite(buff, dev->ph_secsz, count, fildev_fp) != count)
	{
		return (-1);
	}

	return (0);
}

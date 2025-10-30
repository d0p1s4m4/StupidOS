#ifndef FDISK_DEVICE_H
# define FDISK_DEVICE_H 1

# include <stdint.h>

# define DEVICE_IS_OPEN(dev) ((dev)->status & (DEV_OPEN))

enum device_status {
	DEV_OPEN   = 1 << 0,
	DEV_RDONLY = 1 << 1,
};

struct device {
	const char *path;

	uint8_t status;

	uint16_t ph_secsz;
	uint16_t lg_secsz;

	uint64_t size;
	uint64_t ph_secnt;
	uint64_t lg_secnt;
};

int device_open(struct device *dev, const char *path);
int device_close(struct device *dev);
int device_read(struct device *dev, uint8_t *buff, uint64_t lba,
				int count);
int device_write(struct device *dev, const uint8_t *buff, uint64_t lba,
				 int count);

#endif /* !FDISK_DEVICE_H */

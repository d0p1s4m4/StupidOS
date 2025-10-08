#ifndef FDISK_DISK_H
# define FDISK_DISK_H 1

struct disk;
struct device;
struct partition;

struct disk_ops {
	int (*probe)(struct disk *dsk);
};

# define DEF_DISK_OPS(name)										\
	static const struct disk_ops ##name_ops = { ##name_probe }

struct disk_label {
	const char *name;
	const struct disk_ops *ops;

	/* */
	struct disk_label *next;
	struct disk_label **prev;
};

# define DEF_DISK_LABEL(name)						\
	static const struct disk_label ##name_label = { \
		#name, &##name_ops, 0, 0}

struct disk {
	struct device *dev;
	struct disk_label *label;
	struct partition *parts;
};

int disk_init(struct disk *dsk);
int disk_cleanup(struct disk *dsk);

#endif /* !FDISK_DISK_H */

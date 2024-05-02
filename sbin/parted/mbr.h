#ifndef PARTED_MBR_H
# define PARTED_MBR_H 1

# include <stdint.h>

# define MBR_MAGIC0 0x55
# define MBR_MAGIC1 0xAA

# define MBR_PART_BOOTABLE (1 << 7)

enum {
	MBR_PART_TYPE_EMPTY          = 0x00,
	MBR_PART_TYPE_FAT12          = 0x01,
	MBR_PART_TYPE_FAT16          = 0x04,
	MBR_PART_TYPE_GPT_PROTECTIVE = 0xEE,
	MBR_PART_TYPE_EFI            = 0xEF
};

typedef struct partition {
	uint8_t attributes;
	uint8_t chs_start[3];
	uint8_t type;
	uint8_t chs_last_sector[3];
	uint32_t lba_start;
	uint32_t sectors_count;
} __attribute__((packed)) Partition;

typedef struct mbr_header {
	uint8_t raw[436];
	uint8_t uid[10];
	struct partition part[4];
	uint8_t magic[2];
} __attribute__((packed)) MBRHeader;

#endif /* !PARTED_MBR_H */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#define MBR_MAGIC0 0x55
#define MBR_MAGIC1 0xAA

#define MBR_PART_BOOTABLE (1 << 7)

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

static char *prg_name = NULL;
static int dump_info = 0;
static const char *diskpath = NULL;
static FILE *diskfd = NULL;
static MBRHeader header;

static void
dump_partition_info(Partition part)
{
	printf("\tBootable: ");
	if (part.attributes & MBR_PART_BOOTABLE)
	{
		printf("yes\n");
	}
	else
	{
		printf("no\n");	
	}
	printf("\tType: %d\n", part.type);
	printf("\tLBA Start: %d\n", part.lba_start);
	printf("\tSectors: %d\n", part.sectors_count);
}

static void
dump_disk(void)
{
	int idx;

	printf("UID: ");
	for (idx = 0; idx < 10; idx++)
	{
		printf("%02X", header.uid[idx]);
	}
	printf("\n");

	for (idx = 0; idx < 4; idx++)
	{
		printf("Partition %d:\n", idx);
		dump_partition_info(header.part[idx]);
	}
}

static void
usage(int retcode)
{
	if (retcode == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more information.\n", prg_name);
	}
	else
	{
		printf("Usage: %s [-hVd] disk\n", prg_name);
		printf("\t-h\tdisplay this help and exit\n");
		printf("\t-V\toutput version information.\n");
		printf("\t-d\tdump disk information\n");
		printf("\t-o out\twrite to file 'out'\n");
		printf("\t-e\textract parts\n");
		printf("\t-i img\t\n");
		printf("\nReport bugs to <%s>\n", MK_BUGREPORT);
	}

	exit(retcode);
}

static void
version(void)
{
	printf("%s commit %s\n", prg_name, MK_COMMIT);
	exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
	prg_name = argv[0];

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'h':
				usage(EXIT_SUCCESS);
				break;
			case 'V':
				version();
				break;
			case 'd':
				dump_info = 1;
				break;
			default:
				usage(EXIT_FAILURE);
		}

		argv++;
		argc--;
	}

	if (argc <= 1) usage(EXIT_FAILURE);

	diskpath = argv[1];
	diskfd = fopen(diskpath, "rb");
	if (diskfd == NULL)
	{
		fprintf(stderr, "%s: %s\n", diskpath, strerror(errno));
		return (EXIT_FAILURE);
	}

	if (fread(&header, sizeof(uint8_t), sizeof(MBRHeader), diskfd) != sizeof(MBRHeader))
	{
		fprintf(stderr, "%s: can't read mbr header\n", diskpath);
		return (EXIT_FAILURE);
	}
	if (header.magic[0] != MBR_MAGIC0 || header.magic[1] != MBR_MAGIC1)
	{
		fprintf(stderr, "%s: no valid MBR\n", diskpath);
		return (EXIT_FAILURE);
	}

	if (dump_info)
	{
		dump_disk();
	}

	return (EXIT_SUCCESS);
}

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include "mbr.h"

static char *prg_name = NULL;
static int dump_info = 0;
static const char *diskpath = NULL;
static FILE *diskfp = NULL;
static FILE *partfp[4] = {
	NULL,
	NULL,
	NULL,
	NULL
};
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
		printf("\t-p0-3\t\n");
		printf("\t-o out\twrite to file 'out'\n");
		printf("\t-e\textract\n");
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
	char c;
	prg_name = basename(argv[0]);

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
			case 'p':
				c = argv[1][2] - '0';
				argv++;
				argc--;
				if (c != 0 && c != 1 && c != 2 && c != 3)
				{
					usage(EXIT_FAILURE);
				}
				partfp[c] = fopen(argv[2], "rb");
				if (partfp[c] == NULL)
				{
					fprintf(stderr, "%s: %s\n", diskpath, strerror(errno));
					return (EXIT_FAILURE);
				}
				break;
			case 'e':
				break;
			default:
				usage(EXIT_FAILURE);
		}

		argv++;
		argc--;
	}

	if (argc <= 1) usage(EXIT_FAILURE);

	diskpath = argv[1];
	diskfp = fopen(diskpath, "rb");
	if (diskfp == NULL)
	{
		fprintf(stderr, "%s: %s\n", diskpath, strerror(errno));
		return (EXIT_FAILURE);
	}

	if (fread(&header, sizeof(uint8_t), sizeof(MBRHeader), diskfp) != sizeof(MBRHeader))
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <libgen.h>
#include <unistd.h>
#include <fdisk/device.h>
#include <fdisk/disk.h>
#include <fdisk/cmd.h>
#include <fdisk/version.h>
#include "mbr.h"

const char *prg_name = NULL;

static struct device dev;
static struct disk disk;

static const char *partfile[4] = { NULL, NULL, NULL, NULL };

static int do_list = 0;
static int do_script = 0;
static int do_import = 0;
static int do_export = 0;

static void
usage(int retcode)
{
	if (retcode == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more information.\n", prg_name);
	}
	else
	{
		printf("Usage: %s [-hVlsei] [-p0-3 img] disk\n", prg_name);
		printf(" Options:\n");
		printf("  -l\tdump disk information\n");
		printf("  -s\tscript mode\n");
		printf("  -h\tdisplay this help and exit\n");
		printf("  -p0-3 img\tXXX\n");
		printf("  -i\timport\n");
		printf("  -e\textract\n");
		printf("  -V\toutput version information.\n");
		printf("\nReport bugs to <%s>\n", MK_BUGREPORT);
	}

	exit(retcode);
}

static void
version(void)
{
	print_version();

	exit(EXIT_SUCCESS);
}

static int
parted(const char *disk_path)
{
	struct device dev;
	struct disk dsk;
	struct context ctx;

	if (device_open(&dev, disk_path) != 0)
	{
		return (EXIT_FAILURE);
	}

	repl(&ctx);

	device_close(&dev);
	return (EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
	int c;

	prg_name = basename(argv[0]);
	while ((c = getopt(argc, argv, "hVlsp:ei")) != -1)
	{
		switch (c)
		{
		case 'h':
			usage(EXIT_SUCCESS);
			break;
		case 'V':
			version();
			break;
		case 'l':
			do_list = 1;
			break;
		case 's':
			do_script = 1;
			break;
		case 'p':
			/* XXX: maybe there is a better way to do this  */
			if (optind >= argc || (optarg[0] < '0' && optarg[0] > '3'))
			{
				usage(EXIT_FAILURE);
			}
			partfile[optarg[0] - '0'] = argv[optind];
			optind++;
			break;
		case 'e':
			do_export = 0;
			break;
		case 'i':
			do_import = 0;
			break;
		default:
			usage(EXIT_FAILURE);
			break;
		}
	}

	if (optind >= argc)
	{
		usage(EXIT_FAILURE);
	}

	return (parted(argv[optind]));
}

#if 0
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
				if (c < 0 && c > 3)
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
#endif

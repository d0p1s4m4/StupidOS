#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define MAGIC 0x44505453

#define BLOCK_SIZE 512

struct superblock {
	uint32_t magic;
	uint32_t isize;
	uint32_t fsize;
	uint32_t free[100];
	uint8_t nfree;
	uint8_t flock;
	uint8_t ilock;
	uint8_t fmod;
	uint32_t time[2];
} __attribute__((packed));

struct inode {
	uint16_t mode;
	uint8_t nlink;
	uint8_t uid;
	uint8_t gid;
	uint32_t size;
	uint16_t addr[8];
	uint32_t actime[2];
	uint32_t modtime[2];
} __attribute__((packed));

#define INODE_SIZE sizeof(struct inode)

struct file {
	unsigned short inode;
	char filename[14];
};

static const char *prg_name = NULL;
static int verbose = 0;
static const char *device = NULL;
static int devfd = 0;
static off_t device_size = 0;

static int
write_sector(size_t secnum, const uint8_t *data, size_t buff_size)
{
	uint8_t buffer[BLOCK_SIZE];

	if (lseek(devfd, secnum * BLOCK_SIZE, SEEK_SET) != (secnum * BLOCK_SIZE))
	{
		fprintf(stderr, "%s: %s\n", device, strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(buffer, 0, BLOCK_SIZE);
	memcpy(buffer, data, buff_size);

	if (write(devfd, buffer, BLOCK_SIZE) != BLOCK_SIZE)
	{
		fprintf(stderr, "%s: x %s\n", device, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

static void
usage(int retcode)
{
	if (retcode == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more informations.\n", prg_name);
	}
	else
	{
		printf("Usage: %s [-hVv] disk\n", prg_name);
		printf("\t-h\tdisplay this help and exit\n");
		printf("\t-V\toutput version information.\n");
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
	struct stat inf;
	int idx;
	uint8_t buffer[BLOCK_SIZE];
	struct superblock sb;

	prg_name = argv[0];

	while ((argc > 1) && (argv[1][0]) == '-')
	{
		switch (argv[1][1])
		{
			case 'h':
				usage(EXIT_SUCCESS);
				break;
			case 'V':
				version();
				break;
			case 'v':
				verbose=1;
				break;
			default:
				usage(EXIT_FAILURE);
		}
		argv++;
		argc--;
	}

	if (argc <= 1) usage(EXIT_FAILURE);

	device = argv[1];

	if (stat(device, &inf) != 0)
	{
		fprintf(stderr, "%s: %s\n", device, strerror(errno));
		return (EXIT_FAILURE);
	}

	device_size = inf.st_size;

	if (verbose) printf("Device size: %ld (%ld blocks)\n", device_size, device_size/BLOCK_SIZE);

	/*if (device_size/BLOCK_SIZE > 0x7fff)
	{
		fprintf(stderr, "%s: File too large\n", device);
		return (EXIT_FAILURE);
	}*/

	/* */

	devfd = open(device, O_WRONLY);
	if (devfd < 0)
	{
		fprintf(stderr, "%s: %s\n", device, strerror(errno));
		return (EXIT_FAILURE);
	}

	memset(buffer, 0, BLOCK_SIZE);
	for (idx = 0; idx < device_size/BLOCK_SIZE; idx++)
	{
		write_sector(idx, buffer, BLOCK_SIZE);
	}
	if (verbose) printf("Write %ld (%ld bytes)\n", device_size/BLOCK_SIZE, device_size);

	memset(&sb, 0, sizeof(struct superblock));

	sb.magic = MAGIC;
	sb.fsize = device_size/BLOCK_SIZE;

	write_sector(1, (uint8_t *)&sb, sizeof(struct superblock));

	//sb.ninode = (device_size/BLOCK_SIZE) / 8;

	printf("sizeof %lu %lu\n", sizeof(struct superblock), sizeof(struct inode));

	close(devfd);

	return (EXIT_SUCCESS);
}

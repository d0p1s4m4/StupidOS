#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <libgen.h>
#include <errno.h>

typedef struct
{
  uint8_t jmp[3];
  uint8_t OEM[8];
  uint16_t bytes_per_sector;
  uint8_t sectors_per_cluster;
  uint16_t reserved_sectors;
  uint8_t number_of_FATs;
  uint16_t root_entries;
  uint16_t logical_sectors;
  uint8_t media_descriptor;
  uint16_t sectors_per_FAT;
  uint16_t sectors_per_track;
  uint16_t heads_per_cylinder;
  uint32_t hidden_sectors;
  uint32_t total_sectors;

  /* EBPB */
  uint8_t drive_number;
  uint8_t flags;
  uint8_t signature;
  uint32_t serial;
  uint8_t label[11];
  uint8_t fs_type[8];
} __attribute__((packed)) BiosParamBlock;

static const char *prg_name;

static void
fatal(const char *str, ...)
{
  va_list ap;

  fprintf(stderr, "%s: ", prg_name);
  va_start(ap, str);
  vfprintf(stderr, str, ap);
  va_end(ap);
  fprintf(stderr, "\n");

  exit(EXIT_FAILURE);
}

static void
dump(const char *img)
{
  FILE *fp;
  BiosParamBlock bpb;
  char label[12];

  fp = fopen(img, "rb");
  if (fp == NULL) fatal("Can't open '%s': %s", img, strerror(errno));

  if (fread(&bpb, 1, sizeof(BiosParamBlock), fp) != sizeof(BiosParamBlock))
    {
      fatal("Can't read BPB");
    }

  printf("Bytes per logical sector: %hd\n", bpb.bytes_per_sector);
  printf("Logical sectors per cluster: %d\n", bpb.sectors_per_cluster);
  memset(label, 0, 12);
  strncpy(label, bpb.label, 11);
  printf("Label: '%s'\n", label);
  printf("signature: %X\n", bpb.signature);
  fclose(fp);
  exit(EXIT_SUCCESS);
}

static void
usage(int retcode)
{
  if (retcode == EXIT_FAILURE)
    {
      fprintf(stderr, "Try '%s -h' form more information.\n", prg_name);
    }
  else
    {
      printf("Usage: %s [-hV] [-D IMG] [-b BIN]\n", prg_name);
      printf("\t-h\tdisplay this help and exit\n");
      printf("\t-V\toutput version information\n");
      printf("\t-D IMG\tdump fs information from IMG\n");

      printf("\nReport bugs to <%s>\n", MK_BUGREPORT);
    }
  exit(retcode);
}

static void
version(void)
{
  printf("%S commit %s\n", prg_name, MK_COMMIT);
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
	case 'D':
	  argv++;
	  argc--;
	  if (argc <= 1) usage(EXIT_FAILURE);
	  dump(argv[1]);
	  break;
	default:
	  usage(EXIT_FAILURE);
	  break;
	}

      argv++;
      argc--;
    }
  return (EXIT_SUCCESS);
}

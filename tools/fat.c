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

typedef struct
{
  uint8_t  name[8];
  uint8_t  ext[3];
  uint8_t  attr;
  uint16_t reserved0;
  uint16_t creation_time;
  uint16_t creation_date;
  uint16_t access_date;
  uint16_t reserved1;
  uint16_t mod_time;
  uint16_t mod_date;
  uint16_t start;
  uint32_t size;
} __attribute__((packed)) Entry;

static const char *prg_name;
static int sector_size = 512;

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
read_sector(uint8_t *dest, int sect, FILE *fp)
{
  if (fseek(fp, sect*sector_size, SEEK_SET) != 0) goto fatal_sect;
  if (fread(dest, 1, sector_size, fp) != sector_size) goto fatal_sect;

  return;

 fatal_sect:
  fatal("Can't read sector %d", sect);
}

static void
dump_bpb(BiosParamBlock *bpb)
{
  char label[12];

  printf("Bytes per logical sector: %hd\n", bpb->bytes_per_sector);
  sector_size = bpb->bytes_per_sector;
  printf("Logical sectors per cluster: %d\n", bpb->sectors_per_cluster);
  printf("Reserved logical sectors: %hd\n", bpb->reserved_sectors);
  printf("Number of FATs: %d\n", bpb->number_of_FATs);
  printf("Root directory entries: %hd\n", bpb->root_entries);
  printf("Total logical sectors: %hd\n", bpb->logical_sectors);
  printf("Logical sectors per FAT: %hd\n", bpb->sectors_per_FAT);
  printf("Serial: %X\n", bpb->serial);
  memset(label, 0, 12);
  strncpy(label, bpb->label, 11);
  printf("Label: '%s'\n", label);
  printf("signature: %X\n", bpb->signature);
}

static void
dump_entry(Entry *entry)
{
  char filename[9];
  char ext[4];
  int i;

  if (entry->name[0] == 0x0) return;

  memset(filename, 0, 9);
  strncpy(filename, entry->name, 8);
  memset(ext, 0, 4);
  strncpy(ext, entry->ext, 3);
  for (i = 7; i > 0; i--)
    {
      if (filename[i] == ' ')
	{
	  filename[i] = '\0';
	}
      else
	{
	  break;
	}
    }

  printf("%s.%s\n", filename, ext);
}

static void
dump(const char *img)
{
  FILE *fp;
  BiosParamBlock *bpb;
  uint8_t buffer[512];
  int root_start;
  int root_size;
  int i;
  int j;

  fp = fopen(img, "rb");
  if (fp == NULL) fatal("Can't open '%s': %s", img, strerror(errno));

  read_sector(buffer, 0, fp);
  if (buffer[511] != 0xAA || buffer[510] != 0x55)
    {
      fatal("MBR not found");
    }

  bpb = (BiosParamBlock *)buffer;

  dump_bpb(bpb);

  root_start = bpb->sectors_per_FAT * bpb->number_of_FATs;
  root_start += bpb->reserved_sectors;

  root_size = bpb->root_entries / (sector_size >> 0x5);

  for (i = 0; i < root_size; i++)
    {
      read_sector(buffer, root_start + i, fp);
      for (j = 0; j < sector_size; j+=32)
	{
	  dump_entry((Entry *)(buffer + j));
	}
    }

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

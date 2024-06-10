#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#ifndef __stupidos__
# include <libgen.h>
#endif
#include <coff.h>

static const struct {
	int magic;
	char *machine;
} MACHINES[] = {
	{F_MACH_AM33,    "Matsushita AM33"},
	{F_MACH_AMD64,   "Intel amd64"},
	{F_MACH_ARM,     "ARM"},
	{F_MACH_ARMNT,   "ARM Thumb-2"},
	{F_MACH_EBC,     "EFI byte code"},
	{F_MACH_I386,    "Intel 80386"},
	{F_MACH_IA64,    "IA64"},
	{F_MACH_M32R,    "Mitsubishi M32R"},
	/* TODO */
	{F_MACH_UNKNOWN, "unknown"}
};

/* */
static const char *prg_name;
static const char *mach = NULL;

/* */
static int display_header = 0;
static int display_program_header = 0;
static int display_sections = 0;


static void
usage(int retval)
{
	if (retval == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more informations.", prg_name);
	}
	else
	{
		printf("Usage: %s [OPTIONS...] [FILES]\n", prg_name);
		printf("Options are:\n");
		printf("\t-a\tEquivalent to: -H -l -S -s -r -d -v -A -I\n");
		printf("\t-H\tDisplay the COFF file header\n");
	}

	exit(retval);
}

static void
version(void)
{
	printf("%s (%s) %s", prg_name, MK_PACKAGE, MK_COMMIT);
	exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
	FILHDR file_header;
	AOUTHDR aout_header;
	SCNHDR section_header;
	char name[9];
	char *type;
	FILE *fp;
	int idx;
	time_t timedat;

#ifndef __stupidos__
	prg_name = basename(argv[0]);
#else
	prg_name = argv[0];
#endif

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'a':
				display_header = 1;
				display_program_header = 1;
				display_sections = 1;
				break;
			case 'H':
				display_header = 1;
				break;
			case 'h':
				usage(EXIT_SUCCESS);
				break;
			case 'V':
				version();
				break;
		}

		argv++;
		argc--;
	}

	if (argc <= 1) usage(EXIT_FAILURE);

	fp = fopen(argv[1], "rb");
	if (fp == NULL)
	{
		return (EXIT_FAILURE);
	}

	if (display_header)
	{
		if (fread(&file_header, 1, FILHSZ, fp) != FILHSZ)
		{
			return (EXIT_FAILURE);
		}

		for (idx = 0; MACHINES[idx].magic != F_MACH_UNKNOWN; idx++)
		{
			if (MACHINES[idx].magic == file_header.f_magic)
			{
				mach = MACHINES[idx].machine;
				break;
			}
		}
		if (mach == NULL) return (EXIT_FAILURE);

		printf("COFF Header:\n");
		printf("  Machine:                   %s\n", mach);
		printf("  Number of section headers: %hd\n", file_header.f_nscns);
		timedat = file_header.f_timdat;
		printf("  Created at:                %s", ctime(&timedat));
		printf("  Start of Symbol Table:     %d (bytes into file)\n", file_header.f_symptr);
		printf("  Number of Symbols:         %d\n", file_header.f_nsyms);
		printf("  Size of optional header:   %hu\n", file_header.f_opthdr);
		printf("  Flags:                     0x%hx\n\n", file_header.f_flags);
	}

	if (display_sections)
	{
		fseek(fp, file_header.f_opthdr + FILHSZ, SEEK_SET);
		printf("Section Headers:\n");
		printf("[Nr] Name      Type      Address   Offset\n");
		printf("     Size\n");
		for (idx = 0; idx < file_header.f_nscns; idx++)
		{
			if (fread(&section_header, 1, SCNHSZ, fp) != SCNHSZ)
			{
				return (EXIT_FAILURE);
			}

			memset(name, 0, 9);
			memcpy(name, section_header.s_name, 8);
			if (section_header.s_flags & STYP_TEXT)
			{
				type = "TEXT";
			} else if (section_header.s_flags & STYP_DATA)
			{
				type = "DATA";
			}
			else if (section_header.s_flags & STYP_BSS)
			{
				type = "BSS";
			}
			else
			{
				type = "???";
			}
			printf("[%2d] %-8s  %-8s  %08X  %08X\n", idx, name, type, section_header.s_vaddr, section_header.s_scnptr);
			printf("     %08X\n", section_header.s_size);
		}
	}

	return (EXIT_SUCCESS);
}

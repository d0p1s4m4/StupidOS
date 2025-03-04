#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
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

static const struct {
	int flag;
	char *str;
} FLAGS[] = {
	{F_RELFLG,   "F_RELFLG"},
	{F_EXEC,     "F_EXEC"},
	{F_LNNO,     "F_LNNO"},
	{F_LSYMS,    "F_LSYMS"},
	{F_LITTLE,   "F_LITTLE"},
	{F_BIG,      "F_BIG"},
	{F_SYMMERGE, "F_SYMMERGE"},

	{0, NULL}
};

static const struct {
	int16_t mag;
	char *str;
} AOUTMAG[] = {
	{OMAGIC,  "OMAGIC"},
	{NMAGIC,  "NMAGIC"},
	{ZMAGIC,  "ZMAGIC"},
	{STMAGIC, "STMAGIC"},
	{SHMAGIC, "SHMAGIC"},

	{0, NULL}
};

/* */
static const char *prg_name;
static const char *mach = NULL;
static const char *aoutmag = NULL;

/* */
static int display_header = 0;
static int display_optional_header = 0;
static int display_sections = 0;
static int display_symbol_table = 0;
static int display_relocs = 0;

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
	SYMENT sym_entry;
	RELOC reloc_entry;
	char name[9];
	char *type;
	FILE *fp;
	int idx;
	int j;
	time_t timedat;
	char *string_table;
	size_t sz_stable;

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
				display_optional_header = 1;
				display_symbol_table = 1;
				display_sections = 1;
				//display_relocs = 1;
				break;
			case 'r':
				display_relocs = 1;
				break;
			case 'H':
				display_header = 1;
				break;
			case 's':
				display_symbol_table = 1;
				break;
			case 'S':
				display_sections = 1;
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
		printf("  Flags:                     ");
		for (idx = 0; FLAGS[idx].str != NULL; idx++)
		{
			if (file_header.f_flags & FLAGS[idx].flag)
			{
				printf("%s ", FLAGS[idx].str);
			}
		}
		printf("\n\n");
	}

	if (display_optional_header)
	{
		if (file_header.f_opthdr)
		{
			fread(&aout_header, 1, AOUTHSZ, fp);
			for (idx = 0; AOUTMAG[idx].str != NULL; idx++)
			{
				if (aout_header.magic == AOUTMAG[idx].mag)
				{
					aoutmag = AOUTMAG[idx].str;
					break;
				} 
			}
			printf("A.out header\n");
			printf("  Magic: %s\n", aoutmag);
			printf("  Entry: 0x%08X\n", aout_header.entry);
			printf("\n");
		}
		else
		{
			printf("There are no optional header.\n\n");
		}
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
		printf("\n");
	}

	if (display_relocs)
	{
		for (idx = 0; idx < file_header.f_nscns; idx++)
		{
			fseek(fp, file_header.f_opthdr + FILHSZ + (SCNHSZ * idx), SEEK_SET);
			if (fread(&section_header, 1, SCNHSZ, fp) != SCNHSZ)
			{
				return (EXIT_FAILURE);
			}

			if (section_header.s_relptr == 0 || section_header.s_nreloc == 0) continue;

			memset(name, 0, 9);
			memcpy(name, section_header.s_name, 8);
			printf("Relocation for section '%s' at offset 0x%X contains %hu entries:\n", name, section_header.s_relptr, section_header.s_nreloc);
			printf(" Address  Index    Type\n");
			fseek(fp, section_header.s_relptr, SEEK_SET);
			for (j = 0; j < section_header.s_nreloc; j++)
			{
				fread(&reloc_entry, 1, RELSZ, fp);
				printf(" %08X %08X %04ho\n", reloc_entry.r_vaddr, reloc_entry.r_symndx, reloc_entry.r_type);
			}
			printf("\n");
		}
	}

	if (display_symbol_table)
	{
		fseek(fp, 0L, SEEK_END);
		sz_stable = ftell(fp) - file_header.f_symptr + (SYMESZ * file_header.f_nsyms);
		string_table = malloc(sz_stable);
		fseek(fp, file_header.f_symptr + (SYMESZ * file_header.f_nsyms), SEEK_SET);
		fread(string_table, 1, sz_stable, fp);
		printf("Symbol table contains %d entries:\n", file_header.f_nsyms);
		printf("  Num:    Value  Type Name\n");
		assert(sizeof(SYMENT) == SYMESZ);
		for (idx = 0; idx < file_header.f_nsyms; idx++)
		{
			fseek(fp, file_header.f_symptr + (SYMESZ * idx), SEEK_SET);
			fread(&sym_entry, 1, SYMESZ, fp);
			memset(name, 0, 9);
			memcpy(name, sym_entry.n_name, 8);
			printf("  %2d:    %08x  %hu %d %hd ", idx, sym_entry.n_value, sym_entry.n_type, sym_entry.n_sclass, sym_entry.n_scnum);
			if (sym_entry.n_zeroes)
			{
				printf("%s\n", name);
			}
			else
			{
				printf("%s\n", string_table + sym_entry.n_offset);
			}
		}

	}

	return (EXIT_SUCCESS);
}

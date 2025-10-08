#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <libgen.h>
#include <fas.h>

static char *prg_name = NULL;

static FAS_Hdr hdr;
static char *strtab = NULL;
static FAS_Sym *symtab = NULL;
static uint8_t *psrc = NULL;
static uint32_t *sectab = NULL;
static size_t sec_cnt = 0;
static size_t sym_cnt = 0;

static int display_header = 0;
static int display_sections = 0;
static int display_symbols = 0;

static void
pstr2cstr(char *dst, const uint8_t *src)
{
	size_t psz;

	psz = *src++;
	while (psz > 0)
	{
		*dst++ = *src++;
		psz--;
	}
	*dst = '\0';
}

static void
fas_cleanup(void)
{
	free(sectab);
	sectab = NULL;
	free(psrc);
	psrc = NULL;
	free(symtab);
	symtab = NULL;
	free(strtab);
	strtab = NULL;
}

static int
fas_load_table(uint8_t **dest, size_t offset, size_t size, FILE *fp)
{
	if (size == 0)
	{
		return (0);
	}

	*dest = (uint8_t *)malloc(size);
	if (*dest == NULL)
	{
		return (-1);
	}

	if (fseek(fp, offset, SEEK_SET) != 0)
	{
		return (-1);
	}

	if (fread(*dest, size, 1, fp) != 1)
	{
		return (-1);
	}

	return (0);
}

int
fas_load_file(const char *file)
{
	FILE *fp;

	fp = fopen(file, "rb");
	if (fp == NULL)
	{
		return (-1);
	}

	if (fread(&hdr, sizeof(FAS_Hdr), 1, fp) != 1)
	{
		goto err;
	}

	if (hdr.magic != FAS_MAGIC)
	{
		goto err;
	}

	if (fas_load_table((uint8_t **)&strtab, hdr.strtab_off,
					   hdr.strtab_len, fp) != 0)
	{
		goto err_cleanup;
	}

	if (fas_load_table((uint8_t **)&symtab, hdr.symtab_off,
					   hdr.symtab_len, fp) != 0)
	{
		goto err_cleanup;
	}

	if (fas_load_table(&psrc, hdr.psrc_off, hdr.psrc_len, fp) != 0)
	{
		goto err_cleanup;
	}

	if (fas_load_table((uint8_t **)&sectab, hdr.sectab_off,
					   hdr.sectab_len, fp) != 0)
	{
		goto err_cleanup;
	}

	sym_cnt = hdr.symtab_len / sizeof(FAS_Sym);
	sec_cnt = hdr.sectab_len / sizeof(uint32_t);

	fclose(fp);
	return (0);

err_cleanup:
	fas_cleanup();
err:
	fclose(fp);
	return (-1);
}

static void
print_header(void)
{
	printf("FAS Header:\n");
	printf("  Magic:                       %" PRIX32 "\n", hdr.magic);
	printf("  Fasm version:                %" PRIu8 ".%" PRIu8 "\n",
		   hdr.ver_major, hdr.ver_minor);
	printf("  Header Length:               %" PRIu16 "\n", hdr.length);
	printf("  Input file:                  %s\n", strtab + hdr.ifnm_off);
	printf("  Output file:                 %s\n", strtab + hdr.ofnm_off);
	printf("  String table offset:         0x%08" PRIX32 "\n",
		   hdr.strtab_off);
	printf("  String table length:         %" PRIu32 "\n", hdr.strtab_len);
	printf("  Symbol table offset:         0x%08" PRIX32 "\n",
		   hdr.symtab_off);
	printf("  Symbol table length:         %" PRIu32 "\n", hdr.symtab_len);
	printf("  Preprocessed sources offset: 0x%08" PRIX32 "\n",
		   hdr.psrc_off);
	printf("  Preprocessed sources length: %" PRIu32 "\n", hdr.psrc_len);
	printf("  ASM dump offset:             0x%08" PRIX32 "\n",
		   hdr.asmdmp_off);
	printf("  ASM dump length:             %" PRIu32 "\n", hdr.asmdmp_len);
	printf("  Section table offset:        0x%08" PRIX32 "\n",
		   hdr.sectab_off);
	printf("  Section table length:        %" PRIu32 "\n", hdr.sectab_len);
	printf("  Symref table offset:         0x%08" PRIX32 "\n",
		   hdr.symref_off);
	printf("  Symref table length:         %" PRIu32 "\n", hdr.symref_len);

	printf("\n");
}

static void
print_sections(void)
{
	size_t idx;

	printf("Section names:\n");
	for (idx = 0; idx < sec_cnt; idx++)
	{
		printf("  %s\n", strtab + sectab[idx]);
	}
	printf("\n");
}

static void
print_symbol_section(uint8_t type, uint32_t rel)
{
	if (type == FAS_ABS)
	{
		printf("ABS ");
		return;
	}

	if (rel & (1<<31))
	{
		printf("UND ");
	}
	else
	{
		printf("%-3" PRIu32 " ", rel);
	}
}

static void
print_symbol_name(uint32_t off)
{
	char name_str[256] = {0};

	if (off == 0)
	{
		return;
	}
	if (off & (1<<31))
	{
		printf("%s", strtab + (off & ~(1<<31)));
	}
	else
	{
		pstr2cstr(name_str, psrc + off);
		printf("%s", name_str);
	}
}

static void
print_symbols(void)
{
	size_t idx;

	printf("Symbol table:\n");
	printf("   Num: Value              Size Ndx Name\n");

	for (idx = 0; idx < sym_cnt; idx++)
	{
		printf("%6zu: ", idx);
		printf("0x%016" PRIx64 " ", symtab[idx].value);
		printf("%-4" PRIu8 " ", symtab[idx].size);
		print_symbol_section(symtab[idx].type, symtab[idx].reloc);
		print_symbol_name(symtab[idx].name_off);
		printf("\n");
	}
	printf("\n");
}

static void
version(void)
{
	printf("%s (%s) %s\n", prg_name, MK_PACKAGE, MK_COMMIT);
	exit(EXIT_SUCCESS);
}

static void
usage(int retval)
{
	if (retval == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more informations.\n",
				prg_name);
	}
	else
	{
		printf("Usage: %s [-aHsS] file\n", prg_name);
	}

	exit(retval);
}

static int
process_file(char *file)
{
	if (fas_load_file(file) != 0)
	{
		return (EXIT_FAILURE);
	}

	if (display_header)
	{
		print_header();
	}

	if (display_sections)
	{
		print_sections();
	}

	if (display_symbols)
	{
		print_symbols();
	}

	fas_cleanup();

	return (EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
	int c;

	prg_name = basename(argv[0]);

	while ((c = getopt(argc, argv, "hHVSsa")) != -1)
	{
		switch (c)
		{
		case 'h':
			usage(EXIT_SUCCESS);
			break;
		case 'V':
			version();
			break;
		case 'H':
			display_header = 1;
			break;
		case 's':
			display_symbols = 1;
			break;
		case 'S':
			display_sections = 1;
			break;
		case 'a':
			display_header = 1;
			display_symbols = 1;
			display_sections = 1;
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

	return (process_file(argv[optind]));
}

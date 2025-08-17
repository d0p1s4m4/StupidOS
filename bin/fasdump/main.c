#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <fas.h>

struct buff {
	size_t size;
	uint8_t *data;
};

static char *prg_name = "fasdump";
static struct fas_header hdr;
static struct buff strtab = { 0, NULL };
static struct buff symtab = { 0, NULL };
static struct buff psrc = { 0, NULL };
static struct buff asmdmp = { 0, NULL };
static struct buff sectab = { 0, NULL };
static struct buff symref = { 0, NULL };

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
		fprintf(stderr, "Try '%s -h' for more informations.", prg_name);
	}

	exit(retval);
}

static int
readall(uint8_t *dest, size_t size, FILE *fp)
{
	size_t total;
	size_t byte_read;

	total = 0;
	do
	{
		byte_read = fread(dest + total, 1, size - total, fp);
		if (byte_read == 0)
		{
			return (-1);
		}

		total += byte_read;
	}
	while (total < size);

	return (0);
}

static int
load_table(struct buff *dst, size_t off, size_t len, FILE *fp)
{
	dst->size = len;
	if (len == 0)
	{
		return (0);
	}

	dst->data = (uint8_t *)malloc(len);
	if (dst->data == NULL)
	{
		return (-1);
	}

	if (fseek(fp, off, SEEK_SET) != 0)
	{
		goto clean_up;
	}

	if (readall(dst->data, len, fp) != 0)
	{
		goto clean_up;
	}

	return (0);

clean_up:
	free(dst->data);
	dst->size = 0;
	return (-1);
}

static int
read_header(FILE *fp)
{
	if (readall((uint8_t *)&hdr, sizeof(struct fas_header), fp) != 0)
	{
		return (-1);
	}

	if (hdr.magic != FAS_MAGIC)
	{
		return (-1);
	}

	return (0);
}

static inline char *
strtab_get(size_t off)
{
	if (off > strtab.size)
	{
		return (NULL);
	}

	return (strtab.data + off);
}

static void
print_header(void)
{
	printf("FAS Header:\n");
	printf("  Magic:                %X\n", hdr.magic);
	printf("  Fasm version:         %d.%d\n", hdr.ver_major,
		   hdr.ver_minor);
	printf("  Length:               %hu\n", hdr.length);
	printf("  Input file:           %s\n", strtab_get(hdr.ifnm_off));
	printf("  Output file:          %s\n", strtab_get(hdr.ofnm_off));
	printf("  String table offset:  0x%08X\n", hdr.strtab_off);
	printf("  String table length:  %u\n", hdr.strtab_len);
	printf("  Symbol table offset:  0x%08X\n", hdr.symtab_off);
	printf("  Symbol table length:  %u\n", hdr.symtab_len);
	printf("  ASM dump offset:      0x%08X\n", hdr.asmdmp_off);
	printf("  ASM dump length:      %d\n", hdr.asmdmp_len);
	printf("  Section table offset: 0x%08X\n", hdr.sectab_off);
	printf("  Section table length: %d\n", hdr.sectab_len);
	printf("  Symref table offset:  0x%08X\n", hdr.symref_off);
	printf("  Symref table length:  %d\n", hdr.symref_len);

	printf("\n");
}

static void
print_sections(void)
{
	size_t seccnt;
	size_t idx;
	uint32_t *sec;

	seccnt = sectab.size / sizeof(uint32_t);
	sec = (uint32_t *)sectab.data;
	printf("Section names:\n");
	for (idx = 0; idx < seccnt; idx++)
	{
		printf("  %s\n", strtab_get(sec[idx]));
	}
	printf("\n");
}

static void
pstr2cstr(char *dst, const char *src)
{
	size_t plen;

	plen = *src++;

	while (plen > 0)
	{
		*dst++ = *src++;
		plen--;
	}
	*dst = '\0';
}

static void
print_sym_name(uint32_t off)
{
	char name_str[255] = {0};

	if (off == 0)
	{
		return;
	}
	if (off & (1<<31))
	{
		strncpy(name_str, strtab_get(off & ~(1<<31)), 255);
		name_str[254] = '\0';
	}
	else
	{
		pstr2cstr(name_str, psrc.data + off);
	}

	printf("%s", name_str);
}

static void
print_sym_section(uint8_t type, uint32_t rel)
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
		printf("%-3" PRIu32 " %-2" PRIu8 " ", rel, type);
	}
}
/*
static const char *
get_symbol_type(uint8_t type)
{
	switch (type)
	{
	case FAS_ABS:
		return ("ABS");
	case FAS_REL_32:
		return ("REL_32");
	default:
		break;
	}

	return ("???");
}
*/
static void
print_symbols(void)
{
	size_t symcnt;
	size_t idx;
	struct fas_symbol *sym;

	printf("Symbol table:\n");
	symcnt = symtab.size / sizeof(struct fas_symbol);
	sym = (struct fas_symbol *)symtab.data;

	printf("   Num: Value              Size Ndx Name\n");

	for (idx = 0; idx < symcnt; idx++)
	{
		printf("%6d: ", idx);
		printf("0x%016" PRIx64 " ", sym[idx].value);
		printf("%-4" PRIu8 " ", sym[idx].size);
		//printf("%s ", get_symbol_type(sym[idx].type));
		print_sym_section(sym[idx].type, sym[idx].reloc);
		print_sym_name(sym[idx].name_off);
		printf("\n");
/*
		printf("\t %04" PRIx16 " : ", sym[idx].flags);
		if (sym[idx].flags & FAS_SYM_DEF) printf("DEF ");
		if (sym[idx].flags & FAS_SYM_ASM_TIME) printf("ASM_TIME ");
		if (sym[idx].flags & FAS_SYM_NOT_FWD_REF) printf("NOT_FWD ");
		if (sym[idx].flags & FAS_SYM_USED) printf("USED ");
		if (sym[idx].flags & FAS_SYM_PRD_USED) printf("PRD_USED ");
		if (sym[idx].flags & FAS_SYM_LPRD_USED) printf("LPRD_USED ");
		if (sym[idx].flags & FAS_SYM_PRD_DEF) printf("PRD_DEF ");
		if (sym[idx].flags & FAS_SYM_LPRD_DEF) printf("LPRD_DEF ");
		if (sym[idx].flags & FAS_SYM_OPT_ADJ) printf("OPT_ADJ ");
		if (sym[idx].flags & FAS_SYM_TWO_CMPLMNT) printf("TWO_CMPLMNT ");
		if (sym[idx].flags & FAS_SYM_MARKER) printf("MARKER ");
		printf("\n");
*/
		/*printf("%X\n", sym[idx].value);*/
	}
	printf("\n");
}

static void
print_asm_dump(void)
{
	struct fas_asmdmp *dmp;

	dmp = (struct fas_asmdmp *)asmdmp.data;
	printf(" 0x%" PRIx32 " 0x%" PRIx64 "\n", dmp->of_off, dmp->addr);
}

static int
process_file(char *fname)
{
	FILE *fp;

	fp = fopen(fname, "rb");
	if (fp == NULL)
	{
		return (EXIT_FAILURE);
	}

	read_header(fp);
	load_table(&strtab, hdr.strtab_off, hdr.strtab_len, fp);
	load_table(&symtab, hdr.symtab_off, hdr.symtab_len, fp);
	load_table(&psrc, hdr.psrc_off, hdr.psrc_len, fp);
	load_table(&asmdmp, hdr.asmdmp_off, hdr.asmdmp_len, fp);
	load_table(&sectab, hdr.sectab_off, hdr.sectab_len, fp);

	print_header();
	print_sections();
	print_symbols();
	print_asm_dump();

	fclose(fp);

	return (EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
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
		default:
			usage(EXIT_FAILURE);
			break;
		}

		argv++;
		argc--;
	}

	if (argc <= 1) usage(EXIT_FAILURE);

	return (process_file(argv[1]));
}

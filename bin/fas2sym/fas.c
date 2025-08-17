#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <fas.h>
#include <elf.h>
#include "fas2sym.h"

static FAS_Hdr fas_hdr;
static char *fas_strtab = NULL;
static FAS_Sym *fas_symtab = NULL;
static uint8_t *fas_psrc = NULL;
static size_t fas_sym_cnt = 0;

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

static int
fas_load_table(uint8_t **dest, size_t offset, size_t size, FILE *fp)
{
	if (size == 0)
	{
		return (0); /* XXX */
	}

	*dest = (uint8_t *)malloc(size);
	if (*dest == NULL)
	{
		msg_err(NULL);
		return (-1);
	}

	if (fseek(fp, offset, SEEK_SET) != 0)
	{
		msg_err(NULL);
		return (-1);
	}

	if (fread(*dest, size, 1, fp) != 1)
	{
		msg_errx("An unexpected error/eof occured while reading");
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
		msg_err(file);
		return (-1);
	}

	if (fread(&fas_hdr, sizeof(FAS_Hdr), 1, fp) != 1)
	{
		msg_errx("%s: unexpected error/eof while reading FAS header",
				 file);
		goto err;
	}

	if (fas_hdr.magic != FAS_MAGIC)
	{
		msg_errx("%s: invalid magic, got '%08" PRIX32
				 "' instead of '%08" PRIX32 "'",
				 file, (uint32_t)FAS_MAGIC);
		goto err;
	}

	if (fas_load_table((uint8_t **)&fas_strtab, fas_hdr.strtab_off,
					   fas_hdr.strtab_len, fp) != 0)
	{
		msg_errx("%s: can't load string table", file);
		goto err_cleanup;
	}

	if (fas_load_table((uint8_t **)&fas_symtab, fas_hdr.symtab_off,
					   fas_hdr.symtab_len, fp) != 0)
	{
		msg_errx("%s: can't load symbol table", file);
		goto err_cleanup;
	}

	if (fas_load_table(&fas_psrc, fas_hdr.psrc_off, fas_hdr.psrc_len,
					   fp) != 0)
	{
	    msg_errx("%s: can't load preprocessed sources", file);
		goto err_cleanup;
	}

	fas_sym_cnt = fas_hdr.symtab_len / sizeof(FAS_Sym);

	msg_verbose(1, "%zu symbol(s) loaded from %s", fas_sym_cnt, file);

	fclose(fp);
	return (0);

err_cleanup:
	fas_cleanup();
err:
	fclose(fp);
	return (-1);
}

static uint32_t
fas_export_symbol_name(uint32_t name_off)
{
	size_t idx = 0;
	char *ptr;
	char name[256] = { 0 };

	if (name_off == 0)
	{
		return (0); /* anonymous symbol */
	}

	if (name_off & (1<<31)) /* if in fas strtab */
	{
		ptr = fas_strtab + (name_off & ~(1<<31)); /* XXX */
	}
	else /* otherwhise it's in psrc */
	{
		pstr2cstr(name, fas_psrc + name_off);
		ptr = name;
	}

	if (elf_add_str(ptr, strlen(ptr) + 1, &idx) != 0)
	{
		/* error occured */
		return (0);
	}

	return (idx);
}

void
fas_export_symbols(void)
{
	size_t idx;
	char *name_ptr;
    size_t name_idx;

	/* export file name symbol */
	name_ptr = fas_strtab + fas_hdr.ifnm_off;
	elf_add_str(name_ptr, strlen(name_ptr) + 1, &name_idx);
	elf_add_symbol(name_idx, 0, 0, ELF32_ST_INFO(STB_LOCAL, STT_FILE),
				   SHN_ABS);

	/* export other symbol */
	for (idx = 0; idx < fas_sym_cnt; idx++)
	{
		if ((fas_symtab[idx].flags & FAS_SYM_DEF) == 0 ||
			fas_symtab[idx].flags & FAS_SYM_ASM_TIME)
		{
			continue;
		}

		name_idx = fas_export_symbol_name(fas_symtab[idx].name_off);

		elf_add_symbol(name_idx, fas_symtab[idx].value, 0,
					   ELF32_ST_INFO(STB_LOCAL, STT_OBJECT), SHN_ABS);
	}

}

void
fas_cleanup(void)
{
	free(fas_psrc);
	fas_psrc = NULL;
	free(fas_symtab);
	fas_symtab = NULL;
	free(fas_strtab);
	fas_strtab = NULL;
}

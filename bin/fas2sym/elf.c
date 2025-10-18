#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include "fas2sym.h"

enum sections {
	SEC_NULL   = 0,
	SEC_TEXT,
	SEC_SHSTRTAB,
	SEC_STRTAB,
	SEC_SYMTAB,
	SEC_END
};

static Elf32_Ehdr elf_ehdr = {
	{
		ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3,
		ELFCLASS32, ELFDATA2LSB, EV_CURRENT,
		0, 0, 0, 0, 0, 0, 0, 0, 0
	},
	ET_DYN, EM_386, EV_CURRENT, 0, 0, 0, 0,
    sizeof(Elf32_Ehdr), sizeof(Elf32_Phdr), 0,
	sizeof(Elf32_Shdr), SEC_END, SEC_SHSTRTAB
};

static Elf32_Shdr elf_shdrs[SEC_END] = {
	{ 0, SHT_NULL, 0, 0, 0, 0, SHN_UNDEF, 0, 0, 0 },
	/* .text */
	{
		1, SHT_NOBITS, SHF_ALLOC | SHF_EXECINSTR,
		0, 0, 0, SHN_UNDEF, 0, 1, 0
	},
	/* .shstrtab */
	{ 7, SHT_STRTAB, 0, 0, 0, 0, SHN_UNDEF, 0, 1, 0 },
	/* .strtab */
	{ 17, SHT_STRTAB, 0, 0, 0, 0, SHN_UNDEF, 0, 1, 0 },
	/* .symtab */
	{
		25, SHT_SYMTAB, 0, 0, 0, 0,
		SEC_STRTAB, 0, 4, sizeof(Elf32_Sym)
	}
};

static struct buffer elf_shstrtab = { 0, 0, NULL };
static struct buffer elf_strtab = { 0, 0, NULL };
static struct buffer elf_symtab = { 0, 0, NULL };

static int
elf_shstrtab_init(void)
{
	const uint8_t initial[] = "\0.text\0.shstrtab\0.strtab\0.symtab\0";

	return (buffer_put(&elf_shstrtab, initial, sizeof(initial), NULL));
}

static int
elf_strtab_init(void)
{
	const uint8_t initial[] = "\0";

	return (buffer_put(&elf_strtab, initial, sizeof(initial), NULL));
}

static int
elf_symtab_init(void)
{
	const Elf32_Sym initial = { 0, 0, 0, 0, 0, SHN_UNDEF };

	return (buffer_put(&elf_symtab, (uint8_t *)&initial,
					   sizeof(Elf32_Sym), NULL));
}

int
elf_init(void)
{
	if (elf_shstrtab_init() != 0)
	{
		return (-1);
	}

	if (elf_strtab_init() != 0)
	{
		return (-1);
	}

	if (elf_symtab_init() != 0)
	{
		return (-1);
	}

	return (0);
}

int
elf_add_symbol(uint32_t name_off, uint32_t value, uint32_t size,
			   uint8_t info, uint16_t sect)
{
	Elf32_Sym sym = { 0 };

	sym.st_name = name_off;
	sym.st_value = value;
	sym.st_size = size;
	sym.st_info = info;
	sym.st_shndx = sect;

	msg_verbose(2, "add symbol: %s",
				(char *)(elf_strtab.data + name_off));

	elf_shdrs[SEC_SYMTAB].sh_info += 1;

	return (buffer_put(&elf_symtab, (uint8_t *)&sym,
					   sizeof(Elf32_Sym), NULL));
}

int
elf_add_str(const char *str, size_t len, size_t *idx)
{
	return (buffer_put(&elf_strtab, (uint8_t *)str, len, idx));
}

static int
elf_write_tab(struct buffer *buff, FILE *fp)
{
	if (buff->cnt == 0 || buff->data == NULL)
	{
		return (0);
	}

	if (fwrite(buff->data, buff->cnt, 1, fp) != 1)
	{
		return (-1);
	}

	return (0);
}

static void
elf_compute_section_offsets(void)
{
	size_t offset;

	offset = sizeof(Elf32_Ehdr);
	elf_shdrs[SEC_SHSTRTAB].sh_offset = offset;
	elf_shdrs[SEC_SHSTRTAB].sh_size = elf_shstrtab.cnt;

	offset += elf_shstrtab.cnt;

	elf_shdrs[SEC_STRTAB].sh_offset = offset;
	elf_shdrs[SEC_STRTAB].sh_size = elf_strtab.cnt;

	offset += elf_strtab.cnt;

	elf_shdrs[SEC_SYMTAB].sh_offset = offset;
	elf_shdrs[SEC_SYMTAB].sh_size = elf_symtab.cnt;
}

static void
elf_resolve_symbols(void)
{
	Elf32_Sym *syms;
	char *strtab;
	size_t idx;
	size_t symcnt;
	uint32_t begin = 0;
	uint32_t end = 0;

	if (text_begin == NULL || text_end == NULL)
	{
		return;
	}

	syms = (Elf32_Sym *)elf_symtab.data;
	strtab = (char *)elf_strtab.data;
	symcnt = (elf_symtab.cnt / sizeof(Elf32_Sym));

	for (idx = 0; idx < symcnt; idx++)
	{
		if (strcmp(strtab + syms[idx].st_name, text_begin) == 0)
		{
			begin = syms[idx].st_value;
			msg_verbose(1, "Sym %s: %X\n", text_begin, begin);
		}
		else if (strcmp(strtab + syms[idx].st_name, text_end) == 0)
		{
			end = syms[idx].st_value;
		}

		if (begin != 0 && end != 0)
		{
			break;
		}
	}

	elf_shdrs[SEC_TEXT].sh_addr = begin;
	elf_shdrs[SEC_TEXT].sh_size = end - begin;

	for (idx = 0; idx < symcnt; idx++)
	{
		if (syms[idx].st_value >= begin && syms[idx].st_value <= end)
		{
			syms[idx].st_shndx = SEC_TEXT;
		}
	}
}

int
elf_write(const char *file)
{
	FILE *fp;
	int status;
	size_t offset;

	fp = fopen(file, "wb");
	if (fp == NULL)
	{
	    msg_err(file);
		return (-1);
	}

	status = -1;

	/* compute section headers offset */
	elf_ehdr.e_shoff = sizeof(Elf32_Ehdr);
	elf_ehdr.e_shoff += elf_shstrtab.cnt +  elf_strtab.cnt;
	elf_ehdr.e_shoff += elf_symtab.cnt;

	elf_resolve_symbols();

	if (fwrite(&elf_ehdr, sizeof(Elf32_Ehdr), 1, fp) != 1)
	{
		msg_errx("%s: An error occurred while writting elf header", file);
		goto end;
	}

	if (elf_write_tab(&elf_shstrtab, fp) != 0)
	{
		msg_errx("%s: An error occured while writting .shstrtab data", file);
		goto end;
	}
	if (elf_write_tab(&elf_strtab, fp) != 0)
	{
		msg_errx("%s: An error occured while writting .strtab data", file);
		goto end;
	}

	if (elf_write_tab(&elf_symtab, fp) != 0)
	{
		msg_errx("%s: An error occured while writting .symtab data", file);
		goto end;
	}

	/* section headers */
	elf_compute_section_offsets();
	/* XXX: fix me */
	elf_shdrs[SEC_SYMTAB].sh_info += 1;/*= 0; *//* (elf_symtab.cnt / sizeof(Elf32_Sym)) + 1;*/

	if (fwrite(elf_shdrs, sizeof(Elf32_Shdr), SEC_END, fp) != SEC_END)
	{
		msg_errx("%s: An error occured while writting sections headers", file);
		goto end;
	}

	status = 0;
end:
	fclose(fp);
	return (status);
}

void
elf_cleanup(void)
{
	buffer_cleanup(&elf_shstrtab);
	buffer_cleanup(&elf_strtab);
	buffer_cleanup(&elf_symtab);
}

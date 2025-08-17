#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "fas2sym.h"

enum sections {
	SEC_NULL   = 0,
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
	ET_REL, EM_386, EV_CURRENT, 0, 0, 0, 0,
    sizeof(Elf32_Ehdr), sizeof(Elf32_Phdr), 0,
	sizeof(Elf32_Shdr), SEC_END, SEC_SHSTRTAB
};

static Elf32_Shdr elf_shdrs[SEC_END] = {
	{ 0, SHT_NULL, 0, 0, 0, 0, SHN_UNDEF, 0, 0, 0 },
	/* .shstrtab */
	{ 1, SHT_STRTAB, 0, 0, 0, 0, SHN_UNDEF, 0, 1, 0 },
	/* .strtab */
	{ 11, SHT_STRTAB, 0, 0, 0, 0, SHN_UNDEF, 0, 1, 0 },
	/* .symtab */
	{
		19, SHT_SYMTAB, 0, 0, 0, 0,
		SEC_STRTAB, 0, 4, sizeof(Elf32_Sym)
	}
};

static struct buffer elf_shstrtab = { 0, 0, NULL };
static struct buffer elf_strtab = { 0, 0, NULL };
static struct buffer elf_symtab = { 0, 0, NULL };

static int
elf_shstrtab_init(void)
{
	const uint8_t initial[] = "\0.shstrtab\0.strtab\0.symtab\0";

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
	elf_shdrs[SEC_SYMTAB].sh_info = 0; /* (elf_symtab.cnt / sizeof(Elf32_Sym)) + 1;*/

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

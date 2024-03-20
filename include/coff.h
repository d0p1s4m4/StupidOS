#ifndef COFF_H
# define COFF_H 1

# include <stdint.h>

typedef struct filehdr
{
	uint16_t f_magic;
	uint16_t f_nscns;
	int32_t f_timdat;
	int32_t f_symptr;
	int32_t f_nsyms;
	uint16_t f_opthdr;
	uint16_t f_flags;
} FILHDR;

# define FILHSZ sizeof(FILHDR)

# define F_MACH_I386 0x014c

# define F_RELFLG   0x0001
# define F_EXEC     0x0002
# define F_LNNO     0x0004
# define F_LSYMS    0x0008
# define F_LITTLE   0x0100
# define F_BIG      0x0200
# define F_SYMMERGE 0x1000

typedef struct aouthdr
{
	int16_t magic;
	int16_t vstamp;
	int32_t tsize;
	int32_t dsize;
	int32_t bsize;
	int32_t entry;
	int32_t text_start;
	int32_t data_start;
} AOUTHDR;

# define AOUTHSZ sizeof(AOUTHDR)

# define OMAGIC  0404
# define ZMAGIC  0413
# define STMAGIC 0401
# define SHMAGIC 0443

typedef struct scnhdr
{
	int8_t s_name[8];
	int32_t s_paddr;
	int32_t s_vaddr;
	int32_t s_size;
	int32_t s_scnptr;
	int32_t s_relptr;
	int32_t s_lnnoptr;
	uint16_t s_nreloc;
	uint16_t s_nlnno;
	int32_t s_flags;
} SCNHDR;

# define SCNHSZ sizeof(SCNHDR)

# define STYP_REG    0x000
# define STYP_DSECT  0x001
# define STYP_NOLOAD 0x002
# define STYP_GROUP  0x004
# define STYP_PAD    0x008
# define STYP_COPY   0x010
# define STYP_TEXT   0x020
# define STYP_DATA   0x040
# define STYP_BSS    0x080
# define STYP_INFO   0x200
# define STYP_OVER   0x400
# define STYP_LIB    0x800

typedef struct reloc
{
	uint32_t r_vaddr;
	uint32_t r_symndx;
	uint16_t r_type;
} RELOC;
# define RELSZ 10

# define R_ABS     0x000
# define R_DIR16   0x001
# define R_REL16   0x002
# define R_DIR32   0x006
# define R_PCRLONG 0x024

typedef struct lineno
{
	union
	{
		uint32_t l_symndx;
		uint32_t l_paddr;
	} l_addr;
	uint16_t l_lnno;
} LINENO;

# define LINESZ 6

#endif /* !COFF_H */

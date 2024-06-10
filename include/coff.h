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

# define F_MACH_UNKNOWN   0x0
# define F_MACH_AM33      0x1d3
# define F_MACH_AMD64     0x8664
# define F_MACH_ARM       0x1c0
# define F_MACH_ARMNT     0x1c4
# define F_MACH_EBC       0xebc
# define F_MACH_I386      0x14c
# define F_MACH_IA64      0x200
# define F_MACH_M32R      0x9041
# define F_MACH_MIPS16    0x266
# define F_MACH_MIPSFPU   0x366
# define F_MACH_MIPSFPU16 0x466
# define F_MACH_POWERPC   0x1f0
# define F_MACH_POWERPCFP 0x1f1
# define F_MACH_R4000     0x166
# define F_MACH_RISCV32   0x5032
# define F_MACH_RISCV64   0x5064
# define F_MACH_RISCV128  0x5128
# define F_MACH_SH3       0x1a2
# define F_MACH_SH3DSP    0x1a3
# define F_MACH_SH4       0x1a6
# define F_MACH_SH5       0x1a8
# define F_MACH_THUMB     0x1c2
# define F_MACH_WCEMIPSV2 0x169

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
} __attribute__((packed)) SCNHDR;

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
} __attribute__((packed)) RELOC;
# define RELSZ 10

# define R_ABS     0
# define R_DIR16   01
# define R_REL16   02
# define R_DIR32   06
# define R_SEG12   011
# define R_PCRLONG 024

typedef struct lineno
{
	union
	{
		uint32_t l_symndx;
		uint32_t l_paddr;
	} l_addr;
	uint16_t l_lnno;
} __attribute__((packed)) LINENO;
# define LINESZ 6

# define N_UNDEF (0x0)
# define N_ABS   (-0x1)
# define N_DEBUG (-0x2)

# define C_EFCN    -1   /* physical end of a function */
# define C_NULL    0
# define C_AUTO    1
# define C_EXT     2  /* external symbol */
# define C_STAT    3  /* static */
# define C_REG     4  /* register var */
# define C_EXTDEF  5  /* external definition */
# define C_LABEL   6
# define C_ULABEL  7  /* undefined label */
# define C_MOS     8  /* member of struct */
# define C_ARG     9
# define C_STRTAG  10 /* struct tag */
# define C_MOU     11 /* member of union */
# define C_UNTAG   12 /* union tag */
# define C_TPDEF   13 /* typedef */
# define C_USTATIC 14
# define C_ENTAG   15
# define C_MOE     16
# define C_REGPARM 17
# define C_FIELD   18
# define C_BLOCK   100
# define C_FCN     101
# define C_EOS     102
# define C_FILE    103
# define C_LINE    104
# define C_ALIAS   105
# define C_HIDDEN  106

# define T_NULL   0
# define T_ARG    1
# define T_CHAR   2
# define T_SHORT  3
# define T_INT    4
# define T_LONG   5
# define T_FLOAT  6
# define T_DOUBLE 7
# define T_STRUCT 8
# define T_UNION  9
# define T_ENUM   10
# define T_MOE    11
# define T_UCHAR  12
# define T_USHORT 13
# define T_UINT   14
# define T_ULONG  15

# define DT_NON 0
# define DT_PTR 1
# define DT_FCN 2
# define DT_ARY 3

typedef struct syment
{
	union
	{
		char _n_name[8]; /* symbol name */
		struct
		{
			int32_t _n_zeroes; /* symbol name */
			int32_t _n_offset; /* loc in str table */
		} _n_n;
		uint32_t _n_nptr[2];
	} _n;
	uint32_t n_value;
	int16_t n_scnum;
	uint16_t n_type;
	int8_t n_sclass;
	int8_t n_numaux;
} __attribute__((packed)) SYMENT;

# define SYMESZ 18

# define n_name   _n._n_name
# define n_zeroes _n._n_n._n_zeroes
# define n_offset _n._n_n._n_offset
# define n_nptr  _n._n_nptr[1]

typedef union auxent
{
	struct 
	{
		int32_t x_tagndx;
		union
		{
			struct
			{
				uint16_t x_lnno;
				uint16_t x_size;
			} x_lnsz;
			int32_t x_fsize;
		} x_misc;
		union
		{
			struct
			{
				int32_t x_lnnoptr;
				int32_t x_endndx;
			} x_fcn;
			struct 
			{
				uint16_t x_dimen[4];
			} x_ary;
		} x_fcnary;
		uint16_t x_tvndx;
	} x_sym;
	struct
	{
		char x_fname[14];
	} x_file;
	struct
	{
		int32_t x_scnlen;
		uint16_t x_nreloc;
		uint16_t x_nlinno;
	} x_scn;
	struct
	{
		int32_t x_tvfill;
		uint16_t x_tvlen;
		uint16_t x_tvran[2];
	} x_tv;
} AUXENT;

# define AUXESZ 18

#endif /* !COFF_H */

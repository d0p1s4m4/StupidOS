#ifndef FAS_H
# define FAS_H 1

# include <stdint.h>

# define FAS_MAGIC 0x1A736166

typedef struct fas_header
{
	uint32_t magic;
	uint8_t  ver_major;
	uint8_t  ver_minor;
	uint16_t length;
	uint32_t ifnm_off;
	uint32_t ofnm_off;
	uint32_t strtab_off;
	uint32_t strtab_len;
	uint32_t symtab_off;
	uint32_t symtab_len;
	uint32_t psrc_off;
	uint32_t psrc_len;
	uint32_t asmdmp_off;
	uint32_t asmdmp_len;
	uint32_t sectab_off;
	uint32_t sectab_len;
	uint32_t symref_off;
	uint32_t symref_len;
} FAS_Hdr;

# define FAS_SYM_DEF         0x001
# define FAS_SYM_ASM_TIME    0x002
# define FAS_SYM_NOT_FWD_REF 0x004
# define FAS_SYM_USED        0x008
# define FAS_SYM_PRD_USED    0x010
# define FAS_SYM_LPRD_USED   0x020
# define FAS_SYM_PRD_DEF     0x040
# define FAS_SYM_LPRD_DEF    0x080
# define FAS_SYM_OPT_ADJ     0x100
# define FAS_SYM_TWO_CMPLMNT 0x200
# define FAS_SYM_MARKER      0x400

enum fas_symbol_type
{
	FAS_ABS,
	FAS_REL_SEG,
	FAS_REL_32,
	FAS_REL_R_32,
	FAS_REL_64,
	FAS_GOT_32,
	FAS_PLT_32,
	FAS_PLT_R_32
};

typedef struct fas_symbol
{
	uint64_t value;
	uint16_t flags;
	uint8_t  size;
	uint8_t  type;
	uint32_t ext_SIB;
	uint16_t pass_ldef;
	uint16_t pass_lused;
	uint32_t reloc;
	uint32_t name_off;
	uint32_t psrc_line_off;
} FAS_Sym;

typedef struct fas_psrc_line
{
	uint32_t from;
	uint32_t lineno;
	uint32_t src_off;
	uint32_t macro_off;
	uint8_t  tokens[];
} FAS_Psrc_Line;

enum fas_code
{
	FAS_CODE_16 = 16,
	FAS_CODE_32 = 32,
	FAS_CODE_64 = 64
};

typedef struct fas_asmdmp
{
	uint32_t of_off;
	uint32_t psrc_line_off;
	uint64_t addr;
	uint32_t ext_SIB;
	uint32_t reloc;
	uint8_t  type;
	uint8_t  code;
	uint8_t  virt;
	uint8_t  high;
} FAS_Asmdmp;

enum fas_register
{
	FAS_REG_BX   = 0x23,
	FAS_REG_BP   = 0x25,
	FAS_REG_SI   = 0x26,
	FAS_REG_DI   = 0x27,
	FAS_REG_EAX  = 0x40,
	FAS_REG_ECX  = 0x41,
	FAS_REG_EDX  = 0x42,
	FAS_REG_EBX  = 0x43,
	FAS_REG_ESP  = 0x44,
	FAS_REG_EBP  = 0x45,
	FAS_REG_ESI  = 0x46,
	FAS_REG_EDI  = 0x47,
	FAS_REG_R8D  = 0x48,
	FAS_REG_R9D  = 0x49,
	FAS_REG_R10D = 0x4A,
	FAS_REG_R11D = 0x4B,
	FAS_REG_R12D = 0x4C,
	FAS_REG_R13D = 0x4D,
	FAS_REG_R14D = 0x4E,
	FAS_REG_R15D = 0x4F,
	FAS_REG_RAX  = 0x80,
	FAS_REG_RCX  = 0x81,
	FAS_REG_RDX  = 0x82,
	FAS_REG_RBX  = 0x83,
	FAS_REG_RSP  = 0x84,
	FAS_REG_RBP  = 0x85,
	FAS_REG_RSI  = 0x86,
	FAS_REG_RDI  = 0x87,
	FAS_REG_R8   = 0x88,
	FAS_REG_R9   = 0x89,
	FAS_REG_R10  = 0x8A,
	FAS_REG_R11  = 0x8B,
	FAS_REG_R12  = 0x8C,
	FAS_REG_R13  = 0x8D,
	FAS_REG_R14  = 0x8E,
	FAS_REG_R15  = 0x8F,
	FAS_REG_EIP  = 0x94,
	FAS_REG_RIP  = 0x98,
};

#endif /* !FAS_H */

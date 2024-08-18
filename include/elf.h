#ifndef ELF_H
# define ELF_H 1

# include <stdint.h>

# define ELF_MAG0 0x7F
# define ELF_MAG1 0x45
# define ELF_MAG2 0x4C
# define ELF_MAG3 0x46

# define EI_NIDENT 16

struct elf_header 
{
	uint8_t e_ident[EI_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
};

/* TODO */

#endif /* !ELF_H */

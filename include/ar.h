#ifndef AR_H
# define AR_H

#include <stdint.h>

# define ARMAG "!<arch>\n"
# define SARMAG 8

# define ARFMAG "`\n"

# define AR_SYMTAB_NAME "__.SYMDEF"

# define AR_EXTNAME    "#1/"
# define AR_EXTNAME_SZ 3

typedef struct arhdr {
	uint8_t ar_name[16];
	uint8_t ar_date[12];
	uint8_t ar_uid[6];
	uint8_t ar_gid[6];
	uint8_t ar_mode[8];
	uint8_t ar_size[10];
	uint8_t ar_fmag[2];
} AR_HDR;

# define SAR_HDR 60


#endif /* !AR_H */

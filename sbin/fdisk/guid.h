#ifndef GUID_H
# define GUID_H 1

# include <stdint.h>

typedef struct {
	uint32_t d1;
	uint16_t d2;
	uint16_t d3;
	uint16_t d4;
	uint8_t  d5[6];
} GUID;

int guid_equal(GUID *g1, GUID *g2);

#endif /* !GUID_H */

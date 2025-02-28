#include "ar.h"
#ifndef AR_ARCHIVE_H
# define AR_ARCHIVE_H 1

# include <stdint.h>

struct archive_entry_hdr {
	unsigned offset;
	time_t date;
	int gid;
	int uid;
	uint16_t mode;
	size_t size;
	char name[255];
};

int archive_open(const char *path);
int archive_close(int fd);
int archive_decode_entry_hdr(struct archive_entry_hdr *dest, struct arhdr *src);
int archive_encode_entry_hdr(struct arhdr *dest, struct archive_entry_hdr *src);
int archive_get_entry_hdr(int fd, struct archive_entry_hdr *entry_hdr);
size_t archive_skip_entry(int fd, struct archive_entry_hdr *entry_hdr);
int archive_copy();

#endif /* !AR_ARCHIVE_H */

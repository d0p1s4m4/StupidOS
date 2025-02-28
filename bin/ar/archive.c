#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ar.h>
#include "archive.h"

#ifndef O_BINARY
# define O_BINARY 0
#endif /* !O_BINARY */

# define AR_STR_TO_NUM(to, from, len, buff, base) do { \
		memcpy(buff, from, len); \
		buf[len] = '\0'; \
		to = strtol(buff, NULL, base); \
	} while (0)

static int
read_data(int fd, void *buff, ssize_t size)
{
	ssize_t rbytes;
	ssize_t total;

	total = 0;
	while ((rbytes = read(fd, (uint8_t *)(buff) + total, size - total)) != 0)
	{
		total += rbytes;
		if (total >= size)
		{
			break;
		}
	}

	return (total);
}

int
archive_open(const char *path)
{
	int fd;
	char buff[SARMAG];

	if ((fd = open(path, O_RDWR | O_BINARY)) < 0)
	{
		return (-1);
	}

	if (read_data(fd, buff, SARMAG) != SARMAG)
	{
		goto err;
	}

	if (strncmp(ARMAG, buff, SARMAG) != 0)
	{
		goto err;
	}

	return (fd);
err:
	close(fd);
	return (-1);
}

int
archive_get_entry_hdr(int fd, struct archive_entry_hdr *entry_hdr)
{
	struct arhdr hdr;
	ssize_t res;
	char buf[13];
	char *tmp;

	entry_hdr->offset = lseek(fd, 0, SEEK_CUR);
	res = read_data(fd, &hdr, sizeof(struct arhdr));
	if (res != sizeof(struct arhdr))
	{
		if (res == 0)
		{
			return (0);
		}
		else
		{
			return (-1);
		}
	}

	if (strncmp(ARFMAG, (char *)hdr.ar_fmag, 2) != 0)
	{
		return (-1);
	}

	AR_STR_TO_NUM(entry_hdr->date, hdr.ar_date, 12, buf, 10);
	AR_STR_TO_NUM(entry_hdr->uid, hdr.ar_uid, 6, buf, 10);
	AR_STR_TO_NUM(entry_hdr->gid, hdr.ar_gid, 6, buf, 10);
	AR_STR_TO_NUM(entry_hdr->mode, hdr.ar_mode, 8, buf, 8);
	AR_STR_TO_NUM(entry_hdr->size, hdr.ar_size, 10, buf, 10);

	if (strncmp(AR_EXTNAME, (char *)hdr.ar_name, AR_EXTNAME_SZ) == 0)
	{
		res = atoi((char *)(hdr.ar_name + AR_EXTNAME_SZ));
		read_data(fd, entry_hdr->name, res);

		entry_hdr->name[res] = '\0';
		entry_hdr->size -= res;
	}
	else
	{
		memcpy(entry_hdr->name, hdr.ar_name, 16);
		for (tmp = entry_hdr->name + 15; *tmp == ' '; tmp--)
		{
			*tmp = '\0';
		}
	}

	return (1);
}

size_t
archive_skip_entry(int fd, struct archive_entry_hdr *hdr)
{
	size_t sz;

	sz = hdr->size;
	if (hdr->size % 2 != 0)
	{
		sz++; /* skip 0x0A */
	}
	return (lseek(fd, sz, SEEK_CUR));
}

int
archive_close(int fd)
{
	return (close(fd));
}

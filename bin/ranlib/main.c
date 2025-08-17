#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <coff.h>
#include <ranlib.h>
#include "../ar/archive.h"
#include "ar.h"

struct rsym {
	uint32_t offset;
	char *symstr;

	struct rsym *next;
	struct rsym **prev;
};

#define DEFAULT_MODE 0644

static int opt_U = 0;
static const char *tmp_path = "ranlib.XXXXXX";

static int
temp_archive(void)
{
	char path[19];
	int fd;

	memcpy(path, tmp_path, 19);
	fd = mkstemp(path);
	if (fd < 0)
	{
		perror(path);
		return (-1);
	}
	unlink(path);
	return (fd);
}

static void
coff_addsym(struct rsym **syms, int *symcnt, char *strtab, SYMENT *syment)
{
	struct rsym *sym;
	char nbuff[9];
	char *name;

	if (syment->n_scnum < 1 || syment->n_sclass != C_EXT)
	{
		return;
	}

	sym = malloc(sizeof(struct rsym));
	if (sym == NULL)
	{
		return;
	}

	if (syment->n_zeroes == 0)
	{
		name = strtab + syment->n_offset;
	}
	else
	{
		memcpy(nbuff, syment->n_name, 8);
		nbuff[8] = '\0';

		name = nbuff;
	}

	free(sym);
}

static void
coff_getsym(struct rsym **syms, int *symcnt, int fd, struct archive_entry_hdr *entry)
{
	size_t off;
	FILHDR hdr;
	char *strtab;
	uint32_t strtabsz;
	SYMENT syment;
	int idx;

	off = lseek(fd, 0, SEEK_CUR);
	strtab = NULL;

	if (read(fd, &hdr, FILHSZ) != FILHSZ)
	{
		goto end;
	}

	if (hdr.f_magic != F_MACH_I386 || (hdr.f_flags & F_EXEC) != 0
			|| (hdr.f_flags & F_LSYMS) != 0)
	{
		goto end;
	}

	/* load strtab but first we need to skip symtab */
	lseek(fd, off + hdr.f_symptr, SEEK_SET);
	for (idx = 0; idx < hdr.f_nsyms; idx++)
	{
		if (read(fd, &syment, SYMESZ) != SYMESZ)
		{
			goto end;
		}

		/* skip auxent */
		if (syment.n_numaux)
		{
			lseek(fd, AUXESZ * syment.n_numaux, SEEK_CUR);
		}
	}

	if (read(fd, &strtabsz, sizeof(uint32_t)) != sizeof(uint32_t))
	{
		goto end;
	}

	strtab = malloc(strtabsz);
	if (strtab == NULL)
	{
		goto end;
	}

	if (read(fd, strtab, strtabsz) != strtabsz)
	{
		goto end;
	}

	/* go back to symtab */
	lseek(fd, off + hdr.f_symptr, SEEK_SET);
	for (idx = 0; idx < hdr.f_nsyms; idx++)
	{
		if (read(fd, &syment, SYMESZ) != SYMESZ)
		{
			goto end;
		}
		if (syment.n_numaux)
		{
			lseek(fd, AUXESZ * syment.n_numaux, SEEK_CUR);
		}

		coff_addsym(syms, symcnt, strtab, &syment);
	}
end:
	free(strtab);
	lseek(fd, off, SEEK_SET);
}

static int
ranlib(const char *archive)
{
	int arfd;
	int tmpfd;
	struct rsym *syms;
	int symcnt;
	struct archive_entry_hdr entry;

	syms = NULL;
	symcnt = 0;

	if ((arfd = archive_open(archive)) < 0)
	{
		return (-1);
	}

	if ((tmpfd = temp_archive()) < 0)
	{
		archive_close(arfd);
		return (-1);
	}

	while (archive_get_entry_hdr(arfd, &entry) > 0)
	{
		printf("Entry: %s\n", entry.name);
		if (strcmp(AR_SYMTAB_NAME, entry.name) == 0)
		{
			archive_skip_entry(arfd, &entry);
			continue;
		}

		archive_skip_entry(arfd, &entry);
	}

	close(tmpfd);
	archive_close(arfd);

	return (0);
}

static void
usage(int retval)
{
	if (retval == EXIT_FAILURE)
	{
		fprintf(stderr, "Try 'ranlib -h' for more information.\n");
	}
	else
	{
		printf("Usage: ranlib [-D] [-U] archive...\n");
		printf("\t-h\tdisplay this help and exit\n");
		printf("\t-D\tfill mtime, uid, gid with 0 (DEFAULT)\n");
		printf("\t-U\tinsert the real mtime, uid, gid ...\n");
	}

	exit(retval);
}

int
main(int argc, char **argv)
{
	int fd;
	struct archive_entry_hdr entry_hdr;

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'h':
				usage(EXIT_SUCCESS);
				break;
			case 'D':
				opt_U = 0;
				break;
			case 'U':
				opt_U = 1;
				break;
		}

		argv++;
		argc--;
	}

	if (argc <= 1) usage(EXIT_FAILURE);

	while (argc > 1)
	{
		ranlib(argv[1]);
		argv++;
		argc--;
	}

	return (EXIT_SUCCESS);
}

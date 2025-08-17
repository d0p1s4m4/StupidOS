#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include "fas2sym.h"

static const char *prg_name;
static const char *outfile = "out.sym";
static int verbose = 0;

static void
msg_err_common(const char *fmt, va_list ap)
{
	fprintf(stderr, "%s: ", prg_name);
	if (fmt)
	{
		vfprintf(stderr, fmt, ap);
	}
}

void
msg_errx(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	msg_err_common(fmt, ap);
	fprintf(stderr, "\n");

	va_end(ap);
}

void
msg_err(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	msg_err_common(fmt, ap);
	if (fmt)
	{
		fprintf(stderr, ": ");
	}

	fprintf(stderr, "%s\n", strerror(errno));

	va_end(ap);
}

void
msg_verbose(int level, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	if (level <= verbose)
	{
		vprintf(fmt, ap);
		printf("\n");
	}
	va_end(ap);
}

static void
usage(int retval)
{
	if (retval == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more information.\n",
				prg_name);
	}
	else
	{
		printf("Usage: %s [-o OUTPUT] file\n", prg_name);
	}

	exit(retval);
}

static void
version(void)
{
	printf("%s (%s) %s\n", prg_name, MK_PACKAGE, MK_COMMIT);

	exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
	int c;
	int status;

	prg_name = basename(argv[0]);

	while ((c = getopt(argc, argv, "hvVo:")) != -1)
	{
		switch (c)
		{
		case 'h':
			usage(EXIT_SUCCESS);
			break;
		case 'V':
			version();
			break;
		case 'v':
			verbose++;
			break;
		case 'o':
			outfile = optarg;
			break;
		default:
			usage(EXIT_FAILURE);
			break;
		}
	}

	if (optind >= argc)
	{
		usage(EXIT_FAILURE);
	}

	if (fas_load_file(argv[optind]) != 0)
	{
		return (EXIT_FAILURE);
	}

	status = EXIT_FAILURE;

	if (elf_init() != 0)
	{
		goto cleanup;
	}

	fas_export_symbols();

	if (elf_write(outfile) != 0)
	{
		goto cleanup;
	}

	status = EXIT_SUCCESS;
cleanup:
	elf_cleanup();
	fas_cleanup();

	return (status);
}

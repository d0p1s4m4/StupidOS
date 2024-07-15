#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#ifndef __stupidos__
# include <libgen.h>
#endif
#include <coff.h>
#include <elf.h>

static const char *prg_name;

static void
usage(int retval)
{
	if (retval == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more informations.", prg_name);
	}
	else
	{
		printf("Usage: %s [OPTIONS...] [FILES]\n", prg_name);
		printf("Options are:\n");
	}

	exit(retval);
}

static void
version(void)
{
	printf("%s (%s) %s", prg_name, MK_PACKAGE, MK_COMMIT);
	exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
#ifndef __stupidos__
	prg_name = basename(argv[0]);
#else
	prg_name = argv[0];
#endif

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'h':
				usage(EXIT_SUCCESS);
				break;
			case 'V':
				version();
				break;
		}

		argv++;
		argc--;
	}

	return (EXIT_SUCCESS);
}

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <libgen.h>

#ifndef PATH_MAX
# define PATH_MAX 1024
#endif /* !PATH_MAX */

static char curdir[PATH_MAX] = { 0 };

static const char *prg_name = NULL;

static const char *specfile = NULL;
static const char *basepath = NULL;

static FILE *specfp = NULL;

static void
usage(int retval)
{
	if (retval == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more informations.\n",
				prg_name);
	}
	else
	{
		printf("Usage: %s [-ce] [-p path] [-f spec]\n", prg_name);
		printf(" -c     \tXXX.\n");
		printf(" -e     \tXXXX\n");
		printf(" -f spec\tXXXX\n");
		printf(" -p path\tXXXX\n");
	}
	exit(retval);
}

int
main(int argc, char **argv)
{
	int c;

	prg_name = basename(argv[0]);

	while ((c = getopt(argc, argv, "hf:")) != -1)
	{
		switch (c)
		{
		case 'h':
			usage(EXIT_SUCCESS);
			break;
		case 'f':
			specfile = optarg;
			break;
		case 'p':
			basepath = optarg;
			break;
		default:
			usage(EXIT_FAILURE);
			break;
		}
	}

	if (basepath == NULL)
	{
		basepath = getcwd(curdir, PATH_MAX);
		if (basepath == NULL)
		{
			return (EXIT_FAILURE);
		}
	}

	printf("basepath: %s\n", basepath);

	return (EXIT_SUCCESS);
}

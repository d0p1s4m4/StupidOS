#include <stdio.h>
#include <stdlib.h>

static const char *prg_name = "boot-install";

static void
usage(int retval)
{
	if (retval == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' for more informations.\n", prg_name);
	}
	else
	{
		printf("Usage: %s [-hV]\n", prg_name);
		printf("Flags:\n");
		printf("\t-h\tdisplay this menu.\n");
		printf("\nReport bug to: <%s>\n", MK_BUGREPORT);
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
	
	return (EXIT_SUCCESS);
}

#include <stdlib.h>
#include <stdio.h>
#include <coff.h>

# define BASE_ADDRESS = 0x100000

static char *prg_name;
static char *outfile = "a.out";

typedef struct {
	size_t size;
	uint8_t *raw;
} Section;

typedef struct object {
	size_t section_count;
	Section *sections;

	struct object *next;
} Object;

typedef struct {

} Symtab;

static void
usage(int retcode)
{
	if (retcode == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' form more information.\n", prg_name);
	}
	else
	{
		printf("Usage: %s [-hV] [-o outfile] [OBJS...]\n", prg_name);
		printf("\t-h\tdisplay this help and exit\n");
		printf("\t-V\toutput version information\n");
		printf("\t-o outfile\tout file (default: %s)\n", outfile);
		
		printf("\nReport bugs to <%s>\n", MK_BUGREPORT);
	}
	
	exit(retcode);
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
	FILE *fp;
	FILHDR fhdr;
	SCNHDR shdr;
	uint8_t *buffer;
    SYMENT entry;
	RELOC reloc;
	int idx;

	prg_name = argv[0];

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
			case 'o':
				argv++;
				argc--;
				if (argc <= 1) usage(EXIT_FAILURE);
				outfile = argv[1];
				break;
		}

		argv++;
		argc--;
	}

	if (argc <= 1) usage(EXIT_FAILURE);

	

	return (EXIT_SUCCESS);
}

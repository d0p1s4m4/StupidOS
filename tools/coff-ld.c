#include <stdlib.h>
#include <stdio.h>
#include <coff.h>

static char *prg_name;
static char *outfile = "a.out";

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
	printf("%s commit %s\n", prg_name, MK_COMMIT);
	exit(EXIT_SUCCESS);
}

int
main(int argc, char **argv)
{
	FILE *fp;
	FILHDR fhdr;
	SCNHDR shdr;
	AOUTHDR aout;

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

	fp = fopen(argv[1], "rb");
	if (fp == NULL)
	{
		return (EXIT_FAILURE);
	}

	fread(&fhdr, 1, FILHSZ, fp);
	printf("magic: 0x%hx\n", fhdr.f_magic);
	printf("n section: %hd\n", fhdr.f_nscns);
	printf("symtab: 0x%X\n", fhdr.f_symptr);
	printf("sym entries: %d\n", fhdr.f_nsyms);
	printf("optional hdr size: %hu\n", fhdr.f_opthdr);
	printf("flags: 0x%hx\n", fhdr.f_flags);

	if (fhdr.f_opthdr > 0)
	{
		fread(&aout, 1, AOUTHSZ, fp);
	}

	fread(&shdr, 1, SCNHSZ, fp);
	printf("name: %c%c%c%c%c%c%c%c\n", shdr.s_name[0], shdr.s_name[1],shdr.s_name[2],shdr.s_name[3],shdr.s_name[4],shdr.s_name[5],shdr.s_name[6],shdr.s_name[7]);
	printf("flags: 0x%x\n", shdr.s_flags);

	fclose(fp);

	return (EXIT_SUCCESS);
}

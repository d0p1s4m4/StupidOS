#include <stdlib.h>
#include <stdio.h>
#include <sodium.h>

static char *prg_name;

struct kernel_header {
	uint8_t jump[3];
	uint8_t magic[8];
	uint8_t signature[32];
};

static void
do_keygen()
{
	uint8_t pk[crypto_sign_ed25519_PUBLICKEYBYTES];
	uint8_t sk[crypto_sign_ed25519_SECRETKEYBYTES];
}

static void
usage(int retcode)
{
	if (retcode == EXIT_FAILURE)
	{
		fprintf(stderr, "Try '%s -h' form more information.\n", prg_name);
	}
	else
	{
		printf("Usage: %s [-hV] [sign|verify|keygen]\n", prg_name);
		printf("\t-h\tdisplay this help and exit\n");
		printf("\t-V\toutput version information\n");
		
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
	prg_name = argv[0];

	if (sodium_init() < 0) abort();

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'h':
				usage(EXIT_SUCCESS);
				break;
			case 'V':
				version();
		}

		argv++;
		argc--;
	}

	if (argc <= 1) usage(EXIT_FAILURE);

	return (EXIT_SUCCESS);
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdisk/cmd.h>

#define BUFFSZ  128
#define MAXARGS 6

static char *
readline(const char *prompt)
{
	size_t linsz = 0;
	size_t buffsz;
	char *line = NULL;
	char buffer[BUFFSZ];

	printf(prompt);
	while (fgets(buffer, BUFFSZ, stdin) != NULL)
	{
		buffsz = strlen(buffer);

		line = realloc(line, linsz + buffsz + 1);
		if (line == NULL)
		{
			return (NULL);
		}

		memcpy(line + linsz, buffer, buffsz);
		linsz += buffsz;
		line[linsz] = '\0';
		if (line[linsz - 1] == '\n')
		{
			line[linsz - 1] = '\0';
			return (line);
		}
	}

	free(line);
	return (NULL);
}

static int
parse_cmd(struct context *ctx, char *line)
{
	char *argv[MAXARGS] = { NULL };
	int argc;
	size_t idx;
	char *tmp;

	argc = 0;
	tmp = strtok(line, " \t");
	while (tmp != NULL && argc < MAXARGS)
	{
		argv[argc++] = tmp;
		tmp = strtok(NULL, " \t");
	}

	for (idx = 0; cmds[idx].cmd != NULL; idx++)
	{
		if (strcmp(argv[0], cmds[idx].cmd) == 0)
		{
			if (cmds[idx].cb != NULL)
			{
				return (cmds[idx].cb(ctx, argc, argv));
			}

			printf("not implemented\n");
			return (-1);
		}
	}

	fprintf(stderr, "\e[31mError:\e[0m unknown command '%s'\n", argv[0]);
	return (-1);
}

int
repl_ask(const char *str)
{
	int c = '\n';

	do
	{
		if (c == '\n')
		{
			printf("%s [y/n] ", str);
		}
		c = fgetc(stdin);

		if (c == 'Y' || c == 'y')
		{
			putchar('\n');
			return (1);
		}
		else if (c == 'N' || c == 'n')
		{
			putchar('\n');
			return (0);
		}
	}
	while (c != EOF);

	return (-1);
}

int
repl(struct context *ctx)
{
	char *line;

	while ((line = readline("(fdisk) ")) != NULL)
	{
		if (line[0] == '\0')
		{
			free(line);
			continue;
		}

		parse_cmd(ctx, line);
		free(line);
	}

	return (0);
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdisk/cmd.h>
#include <fdisk/version.h>

static inline void
cmd_help_print_idx(size_t idx)
{
	printf("\e[1m%s\e[0m", cmds[idx].cmd);
	if (cmds[idx].args != NULL)
	{
		printf(" \e[3m%s\e[0m", cmds[idx].args);
	}
	printf("\n\t%s\n", cmds[idx].desc);
}

static int
cmd_help(struct context *ctx, int argc, char **argv)
{
	size_t idx;

	(void)ctx;
	(void)argc;
	(void)argv;

	for (idx = 0; cmds[idx].cmd != NULL; idx++)
	{
		if (argc >= 2)
		{
			if (strcmp(argv[1], cmds[idx].cmd) == 0)
			{
				cmd_help_print_idx(idx);
				return (0);
			}
		}
		else
		{
			cmd_help_print_idx(idx);
		}
	}

	if (argc >= 2)
	{
		printf("\e[31mError:\e[0m Unknown command \"%s\"\n", argv[1]);
		return (-1);
	}
	return (0);
}

static int
cmd_version(struct context *ctx, int argc, char **argv)
{
	(void)ctx;
	(void)argc;
	(void)argv;

    print_version();

	return (0);
}

static int
cmd_print(struct context *ctx, int argc, char **argv)
{
	long disk_sz;

	(void)argc;
	(void)argv;
	fseek(ctx->dsk_fp, 0, SEEK_END);
	disk_sz = ftell(ctx->dsk_fp);
	rewind(ctx->dsk_fp);
	printf("Disk %s: %ldB\n", ctx->dsk_fname, disk_sz);
	return (0);
}

static int
cmd_quit(struct context *ctx, int argc, char **argv)
{
	(void)argc;
	(void)argv;

	if (ctx->dsk_fp != NULL)
	{
		fclose(ctx->dsk_fp);
	}
	exit(EXIT_SUCCESS);

	return (0);
}

static int
cmd_set(struct context *ctx, int argc, char **argv)
{
	return (0);
}

static int
cmd_rm(struct context *ctx, int argc, char **argv)
{
	return (0);
}

static int
cmd_mklabel(struct context *ctx, int argc, char **argv)
{
	return (0);
}


const struct cmd cmds[] = {
	DEF_CMD(help, "[command]", "Display general help or 'command' help"),
	DEF_CMD(version, NULL, "Display version information and copyright message"),
	DEF_CMD(print, NULL, "Display partition table"),
	DEF_CMD(mklabel, "label-type", "Create a new partition table of 'label-type'. 'label-type' should be one of \"msdos\" or \"gpt\"."),
    DEF_CMD(rm, "partition", "Delete 'partition'"),
	DEF_CMD(set, "partition flag [state]", "XXX"),
    DEF_CMD(quit, NULL, "Exit from fdisk"),
	{ NULL, NULL, NULL, NULL }
};

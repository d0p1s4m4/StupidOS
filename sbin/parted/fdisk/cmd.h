#ifndef FDISK_CMD_H
# define FDISK_CMD_H 1

# define FDISK_LINE_MAX 1024

struct context {
    const char *dsk_fname;
	FILE *dsk_fp;

	int script;
};

struct cmd {
	const char *cmd;
	const char *args;
	const char *desc;
	int (*cb)(struct context *, int, char **);
};

# define DEF_CMD(name, args, desc)				\
	{ #name, args, desc, cmd_##name }

/* cmd.c */
extern const struct cmd cmds[];

/* repl.c */
int repl(struct context *ctx);

#endif /* :FDISK_CMD_H */

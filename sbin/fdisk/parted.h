#ifndef PARTED_H
# define PARTED_H 1

# include <stdio.h>

# define PARTED_LINE_MAX 1024

# define PARTED_DEFAULT_SECSZ 512

enum parted_unit {
	PARTED_KiB = 1024,
	PARTED_MiB = 1024 * 1024,
	PARTED_GiB = 1024 * 1024 * 1024,

	PARTED_KB = 1000,
	PARTED_MB = 1000 * 1000,
	PARTED_GB = 1000 * 1000 * 1000,
};

/* copyright.c */
extern const char *copyright;

/* main.c */
extern const char *prg_name;

#endif /* !PARTED_H */

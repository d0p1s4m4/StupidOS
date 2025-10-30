#include <stdio.h>

#ifndef MK_PACKAGE
# define MK_PACKAGE "ext"
#endif /* !MK_PACKAGE */
#ifndef MK_COMMIT
# define MK_COMMIT "????"
#endif /* !MK_COMMIT */

extern const char *prg_name;

static const char *copyright =											\
	"Copyright (c) 2025, d0p1\n"										\
	"License BSD-3-Clause: "											\
	"<https://directory.fsf.org/wiki/License:BSD-3-Clause>\n"			\
	"This is free software: you are free to change and redistribute it.\n" \
	"There is NO WARRANTY, to the extent permitterd by law.\n";

void
print_version(void)
{
	printf("%s (%s) %s\n", prg_name, MK_PACKAGE, MK_COMMIT);
	printf(copyright);
}

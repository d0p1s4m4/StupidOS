#include <stdio.h>
#include <stupidc.h>

#ifndef MK_PACKAGE
# define MK_PACKAGE "StupidOS"
#endif /* !MK_PACKAGE */

#ifndef MK_COMMIT
# define MK_COMMIT "???????"
#endif /* !MK_COMMIT */

void
print_version(void)
{
	printf("%s (%s) %s\n", get_progname(), MK_PACKAGE, MK_COMMIT);
	printf("Copyright (C) 2025, d0p1\n"
		   "License BSD-3-Clause: <https://directory.fsf.org/wiki/License:BSD-3-Clause>\n"
		   "This is free software: you are free to change and redistribute it.\n"
		   "There is NO WARRANTY, to the extent permitterd by law.\n");
}

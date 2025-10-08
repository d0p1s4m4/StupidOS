#include <string.h>
#include <stupidc.h>

#ifdef _WIN32
# define PATH_SEP '\\'
#else
# define PATH_SEP '/'
#endif /* _WIN32 */

static const char *__progname;

void
set_progname(const char *progname)
{
	__progname = strrchr(progname, PATH_SEP);
	if (__progname == NULL)
	{
		__progname = progname;
	}
	else
	{
		__progname++;
	}
}

const char *
get_progname(void)
{
	return (__progname);
}

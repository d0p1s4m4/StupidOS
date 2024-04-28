#ifndef TIME_H
# define TIME_H 1

# include <stdint.h>

/* POSIX define CLOCKS_PER_SEC as on milion */
# define CLOCKS_PER_SEC 1000000

typedef int64_t time_t;
typedef uint64_t clock_t;

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

double difftime(time_t time_end, time_t time_beg);
time_t time(time_t *arg);
clock_t clock(void);
char *asctime(const struct tm *time); /* fuck modern C */
char *ctime(const time_t *timer);
size_t strftime(char *str, size_t count, const char *format, const struct tm *tp);

#endif /* !TIME_H */

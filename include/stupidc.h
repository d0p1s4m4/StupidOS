#ifndef STUPIDC_H
# define STUPIDC_H 1

# include <stddef.h>

# define ARR_BASE_CAP 8

# define ARR(type)									\
	struct { size_t cap; size_t cnt; type *elems; }

# define ARR_INIT { 0, 0, NULL }

# define ARR_GROW(arr)												\
	do {															\
		if ((arr)->cap < ((arr)->cnt + 1))							\
		{															\
			(arr)->cap = ((arr)->cap > ARR_BASE_CAP					\
						  ? (arr)->cap * 2 : ARR_BASE_CAP);			\
			(arr)->elems = realloc((arr)->elems,					\
							  (arr)->cap * sizeof(*(arr)->elems));	\
		}															\
	} while(0)

# define ARR_APPEND(arr, elem)					\
	do {										\
		ARR_GROW((arr));						\
		(arr)->elems[(arr)->cnt++] = (elem);	\
	} while (0)

# define ARR_RESET(arr)							\
	do {										\
		(arr)->cnt = 0;							\
	} while (0)

# define ARR_FREE(arr)			\
	do {						\
		(arr)->cap = 0;			\
		(arr)->cnt = 0;			\
		free((arr)->elems);		\
		(arr)->elems = NULL;	\
	} while(0)

# define ARR_COUNT(arr)       ((arr)->cnt)
# define ARR_EMPTY(arr)       ((arr)->cnt == 0)
# define ARR_INDEX(arr, elem) ((elem) - (arr)->elems)
# define ARR_FIRST(arr)       ((arr)->elems[0])
# define ARR_LAST(arr)        ((arr)->elems[(arr)->cnt])

# define ARR_FOREACH(var, arr)					\
	for ((var) = (arr)->elems;					\
		 (var) < ((arr)->elems + (arr)->cnt);	\
		 (var)++)

# define LIST(type)										\
	struct { struct type *next; struct type **prev }

void set_progname(const char *name);
const char *get_progname(void);

void print_version(void);

#endif /* !STUPIDC_H */

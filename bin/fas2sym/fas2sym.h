#ifndef FAS2SYM_H
# define FAS2SYM_H 1

# include <stddef.h>
# include <stdint.h>
# include <stdarg.h>

struct buffer {
	size_t cap;
	size_t cnt;
	uint8_t *data;
};

/* buffer.c */
int buffer_put(struct buffer *buff, const uint8_t *data,
			   size_t size, size_t *index);
void buffer_cleanup(struct buffer *buff);

/* fas.c */
int fas_load_file(const char *file);
void fas_export_symbols(void);
void fas_cleanup(void);

/* elf.c */
int elf_init(void);
int elf_add_str(const char *str, size_t len, size_t *idx);
int elf_add_symbol(uint32_t name_off, uint32_t value, uint32_t size,
				   uint8_t info, uint16_t sect);
int elf_write(const char *file);
void elf_cleanup(void);

/* main.c */
void msg_err(const char *fmt, ...);
void msg_errx(const char *fmt, ...);
void msg_verbose(int level, const char *fmt, ...);

#endif /* !FAS2SYM_H */

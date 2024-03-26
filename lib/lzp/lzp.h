#ifndef LZP_H
# define LZP_H 1

int lzp_compress(void *out, const void *in, int insz);
int lzp_decompress(void *out, const void *in, int insz);

#endif /* !LZP_H */

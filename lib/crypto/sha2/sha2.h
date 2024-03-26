#ifndef CRYPTO_SHA2_H
# define CRYPTO_SHA2_H 1

# define SHA224_BYTES 28
# define SHA256_BYTES 32
# define SHA384_BYTES 48
# define SHA512_BYTES 64

void sha224(void *out, const void *in, unsigned int len);
void sha256(void *out, const void *in, unsigned int len);

void sha384(void *out, const void *in, unsigned int len);
void sha512(void *out, const void *in, unsigned int len);

#endif /* !CRYPTO_SHA2_H */

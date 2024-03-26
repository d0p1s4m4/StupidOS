#ifndef CHACHA20_H
# define CHACHA20_H 1

# include <stdint.h>

# define CHACHA20_KEY_BYTES 32
# define CHACHA20_NONCE_BYTES 12
# define CHACHA20_BLOCK_BYTES 64

typedef struct
{
	uint32_t state[4][4];
	uint32_t ctr;
} ChaCha20Ctx;

void chacha20_init(ChaCha20Ctx *ctx, const uint8_t *key, uint32_t ctr,
													const uint8_t *nonce);
void chacha20_block(ChaCha20Ctx *ctx, uint8_t *out);

#endif /* !CHACHA20_H */

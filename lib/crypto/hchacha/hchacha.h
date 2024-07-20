#ifndef CRYPTO_HCHACHA_H
# define CRYPTO_HCHACHA_H 1

# include <stdint.h>

# define HCHACHA_OUT_BYTES   32
# define HCHACHA_KEY_BYTES   32
# define HCHACHA_NONCE_BYTES 16

void hchacha(uint8_t out[HCHACHA_OUT_BYTES],
			const uint8_t key[HCHACHA_KEY_BYTES],
			const uint8_t nonce[HCHACHA_NONCE_BYTES], int round);

void hchacha12(uint8_t out[HCHACHA_OUT_BYTES],
			const uint8_t key[HCHACHA_KEY_BYTES],
			const uint8_t nonce[HCHACHA_NONCE_BYTES]);

void hchacha20(uint8_t out[HCHACHA_OUT_BYTES],
			const uint8_t key[HCHACHA_KEY_BYTES],
			const uint8_t nonce[HCHACHA_NONCE_BYTES]);

#endif /* !CRYPTO_HCHACHA_H */

struc ChaCha20Ctx {
	.state dd 4*4 dup(?)
	.ctr   dd ? 
}

CHACHA20_KEY_BYTES   = 32
CHACHA20_NONCE_BYTES = 12
CHACHA20_ROUND       = 10
CHACHA20_BLOCK_BYTES = 64

	public chacha20_init
	public chacha20_block

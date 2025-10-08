#ifndef GPT_H
# define GPT_H 1

# define GPT_MAGIC "EFI PART"

# define GPT_UNUSED_ENTRY { 0x00000000, 0x0000, 0x0000, \
							{ 0x00, 0x00, 0x00, 0x00,   \
									0x00, 0x00, 0x00, 0x00 }}

struct gpt_header {
	uint8_t  magic[8];
	uint32_t revision;
	uint32_t hsize;
	uint32_t hcrc32;
	uint32_t reserved;
	uint64_t curr_LBA;
	uint64_t alt_LBA;
	uint64_t fuse_LBA;
	uint64_t luse_LBA;
	uint8_t  guid[16];
	uint64_t start_LBA;
	uint32_t pentnum;
	uint32_t pentsize;
	uint32_t pentcrc32;
};


#endif /* !GPT_H */

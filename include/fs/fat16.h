#ifndef ECHFS_H
# define ECHFS_H 1

typedef struct {
	uint8_t jmp[4];
	uint8_t signature[8];

} EchFSIdentityTable;

#endif /* !ECHFS_H */

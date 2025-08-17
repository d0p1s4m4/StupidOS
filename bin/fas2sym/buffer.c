#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fas2sym.h"

int
buffer_put(struct buffer *buff, const uint8_t *data, size_t size,
		   size_t *index)
{
	if (index != NULL)
	{
		*index = buff->cnt;
	}

	while (buff->cap < buff->cnt + size)
	{
		buff->cap = (buff->cap == 0 ? 32 : buff->cap * 2);
		buff->data = (uint8_t *)realloc(buff->data, buff->cap);
		if (buff->data == NULL)
		{
			msg_err(NULL);
			return (-1);
		}
	}

	memcpy(buff->data + buff->cnt, data, size);
	buff->cnt += size;

	return (0);
}

void
buffer_cleanup(struct buffer *buff)
{
	buff->cnt = 0;
	buff->cap = 0;
	free(buff->data);
	buff->data = 0;
}

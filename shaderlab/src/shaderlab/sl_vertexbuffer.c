#include "sl_vertexbuffer.h"

#include <stdlib.h>
#include <string.h>

struct sl_vertexbuffer* 
sl_vb_create(int size, int cap) {
	int sz = sizeof(struct sl_vertexbuffer) + size * cap;
	struct sl_vertexbuffer* vb = (struct sl_vertexbuffer*)malloc(sz);
	memset(vb, 0, sz);
	vb->size = size;
	vb->n = 0;
	vb->cap = cap;
	vb->buf = (char*)(vb + 1);
	return vb;
}

void 
sl_vb_release(struct sl_vertexbuffer* vb) {
	free(vb);
}

bool 
sl_vb_add(struct sl_vertexbuffer* vb, void* data, int n) {
	if (vb->n + n >= vb->cap) {
		return true;
	}

	memcpy(vb->buf + vb->size * vb->n, data, vb->size * n);
	vb->n += n;
	return false;
}
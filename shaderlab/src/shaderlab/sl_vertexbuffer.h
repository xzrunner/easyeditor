#ifdef __cplusplus
extern "C"
{
#endif

#ifndef shader_lab_vertexbuffer_h
#define shader_lab_vertexbuffer_h

#include <stdbool.h>

struct sl_vertexbuffer {
	char* buf;

	int size;
	int n;
	int cap;
};

struct sl_vertexbuffer* sl_vb_create(int size, int cap);
void sl_vb_release(struct sl_vertexbuffer*);

bool sl_vb_add(struct sl_vertexbuffer*, void* data, int n);

#endif // shader_lab_vertexbuffer_h

#ifdef __cplusplus
}
#endif
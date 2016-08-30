#ifdef __cplusplus
extern "C"
{
#endif

#ifndef simp_pack_texture_h
#define simp_pack_texture_h

#include "simp_pack.h"

#include <stdint.h>

// todo
struct simp_texture {
	uint32_t type;
};

#define SIZEOF_TEXTURE (sizeof(struct simp_texture))

void simp_import_texture(struct simp_import_stream*);

#endif // simp_pack_texture_h

#ifdef __cplusplus
}
#endif
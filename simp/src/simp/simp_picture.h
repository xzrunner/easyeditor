#ifdef __cplusplus
extern "C"
{
#endif

#ifndef simp_pack_picture_h
#define simp_pack_picture_h

#include <stdint.h>

struct simp_picture {
	int texid;
	uint16_t texcoords[8];
};

#define SIZEOF_PICTURE (sizeof(struct simp_picture))

void simp_import_picture(struct simp_import_stream*);

#endif // simp_pack_picture_h

#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef simp_pack_scale9_h
#define simp_pack_scale9_h

#include <stdint.h>

struct simp_scale9_grid {
	uint32_t sym;
	uint16_t dir;
	uint16_t mirror;
};

#define SIZEOF_SCALE9_GRID (sizeof(struct simp_scale9_grid))

struct simp_scale9 {
	uint32_t type;
	struct simp_scale9_grid grids[1];
};

#define SIZEOF_SCALE9 (sizeof(struct simp_scale9) - sizeof(struct simp_scale9_grid))

void simp_import_scale9(struct simp_import_stream*);

struct simp_scale9_spr {
	uint32_t sym;
	uint16_t width, height;
};

#define SIZEOF_SCALE9_SPR (sizeof(struct simp_scale9_spr))

void simp_import_scale9_spr(struct simp_import_stream*);

#endif // simp_pack_scale9_h

#ifdef __cplusplus
}
#endif
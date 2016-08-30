#include "simp_scale9.h"
#include "simp_import.h"

void 
simp_import_scale9(struct simp_import_stream* is) {
	int n = 0;
	int type = simp_import_uint8(is);
	switch (type) {
	case 1:
		n = 9;
		break;
	case 2: case 3:
		n = 3;
		break;
	case 4:
		n = 6;
		break;
	case 5:
		n = 8;
		break;
	}

	int sz = SIZEOF_SCALE9 + SIZEOF_SCALE9_GRID * n;
	struct simp_scale9* s9 = simp_import_alloc(is, sz);
	s9->type = type;
	for (int i = 0; i < n; ++i) {
		struct simp_scale9_grid* g = &s9->grids[i];
		g->sym = simp_import_uint32(is);
		g->dir = simp_import_uint16(is);
		g->mirror = simp_import_uint16(is);
	}
}

void 
simp_import_scale9_spr(struct simp_import_stream* is) {
	struct simp_scale9_spr* s9 = simp_import_alloc(is, SIZEOF_SCALE9_SPR);
	s9->sym = simp_import_uint32(is);
	s9->width = simp_import_uint16(is);
	s9->height = simp_import_uint16(is);
}
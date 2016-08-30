#ifdef __cplusplus
extern "C"
{
#endif

#ifndef simp_pack_label_h
#define simp_pack_label_h

#include "simp_pack.h"

#include <stdint.h>

struct simp_label {
	int width;
	int height;

	int font;
	int font_size;
	uint32_t font_color;

	int edge;
	float edge_size;
	uint32_t edge_color;

	int align_hori;
	int align_vert;

	float space_hori;
	float space_vert;

	int richtext;
	int overflow;

	const char* text;
	const char* tid;
};

#define SIZEOF_LABEL (sizeof(struct simp_label) + PTR_SIZE_DIFF * 2)

void simp_import_label(struct simp_import_stream*);

#endif // simp_pack_label_h

#ifdef __cplusplus
}
#endif
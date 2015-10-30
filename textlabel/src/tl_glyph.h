#ifdef __cplusplus
extern "C"
{
#endif

#ifndef textlabel_glyph_h
#define textlabel_glyph_h

#include "tl_typedef.h"

#include <stdbool.h>

struct tl_glyph_sizer {
	float width;
	float height;
};

struct tl_glyph_layout {
	struct tl_glyph_sizer sizer;
	float bearing_x, bearing_y;
	float advance;
	float metrics_height;
};

void tl_glyph_cache_init(int cap);

void tl_glyph_cache_clear();

uint8_t* tl_glyph_gen_char(int unicode, int font, int size, bool edge, struct tl_glyph_layout* layout);

#endif // textlabel_glyph_h

#ifdef __cplusplus
}
#endif
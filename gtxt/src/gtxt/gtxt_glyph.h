#ifdef __cplusplus
extern "C"
{
#endif

#ifndef gametext_glyph_h
#define gametext_glyph_h

#include "gtxt_typedef.h"

#include <stdbool.h>

struct gtxt_glyph_sizer {
	float width;
	float height;
};

struct gtxt_glyph_layout {
	struct gtxt_glyph_sizer sizer;
	float bearing_x, bearing_y;
	float advance;
	float metrics_height;
};

void gtxt_glyph_cache_init(int cap_bitmap, int cap_layout);
//void gtxt_glyph_cache_clear();

struct gtxt_glyph_layout* gtxt_glyph_get_layout(int unicode, int font, int size, bool edge);

uint8_t* gtxt_glyph_get_bitmap(int unicode, int font, int size, bool edge, struct gtxt_glyph_layout* layout);

#endif // gametext_glyph_h

#ifdef __cplusplus
}
#endif
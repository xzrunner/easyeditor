#ifndef _DRAG2D_GLYPH_UTIL_H_
#define _DRAG2D_GLYPH_UTIL_H_

#include <stdint.h>

namespace d2d
{

union Pixel32
{
	uint32_t integer;
	struct
	{
		uint8_t r, g, b, a;
	};

	void Set(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
		r = _r; g = _g; b = _b; a = _a;
	}
};

struct GlyphSizer
{
	float width;
	float height;	

	GlyphSizer() : width(0), height(0) {}
	GlyphSizer(float w, float h) : width(w), height(h) {}
};

struct GlyphLayout
{
	GlyphSizer sizer;
	float bearingX;
	float bearingY;
	float advance;
	float metrics_height;

	GlyphLayout() : bearingX(0), bearingY(0), advance(0), metrics_height(0) {}
};

}

#endif // _DRAG2D_GLYPH_UTIL_H_
#ifndef _DRAG2D_GLYPH_UTIL_H_
#define _DRAG2D_GLYPH_UTIL_H_

namespace d2d
{

struct GlyphSizer
{
	int width;
	int height;
	int bearingX;
	int bearingY;
	int advance;
	int metrics_height;
};

}

#endif // _DRAG2D_GLYPH_UTIL_H_
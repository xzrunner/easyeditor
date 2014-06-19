#ifndef _DRAG2D_FREETYPE_RENDER_H_
#define _DRAG2D_FREETYPE_RENDER_H_

#include "glyph_util.h"

#include <ft2build.h>
#include <freetype/freetype.h>

namespace d2d
{

struct GlyphLayout;

class FTRender
{
public:
	FTRender();
	~FTRender();

	void LoadFont(const char* filename);

	GlyphSizer GetGlyphSizer(int unicode, int size);

	uint32_t* WriteGlyphWithStroker(
		int unicode,
		int size,
		union Pixel32 fontCol,
		union Pixel32 outlineCol,
		float outlineWidth,
		GlyphLayout& layout
		);

	uint32_t* WriteGlyphNoStroker(
		int unicode,
		int size,
		union Pixel32 col,
		GlyphLayout& layout
		);

private:
	FT_Library m_library;
	FT_Face m_face;

}; // FTRender

}

#endif // _DRAG2D_FREETYPE_RENDER_H_
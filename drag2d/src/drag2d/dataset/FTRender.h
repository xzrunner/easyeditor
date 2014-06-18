#ifndef _DRAG2D_FREETYPE_RENDER_H_
#define _DRAG2D_FREETYPE_RENDER_H_

#include "common/tools.h"

#include <ft2build.h>
#include <freetype/freetype.h>

namespace d2d
{

struct GlyphSizer;

class FTRender
{
public:
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

public:
	FTRender();
	~FTRender();

	void LoadFont(const char* filename);

	//void GetGlyphSize(int unicode, int size, int* width, int* height);

	unsigned int* WriteGlyphWithStroker(
		int unicode,
		int size,
		union Pixel32 fontCol,
		union Pixel32 outlineCol,
		float outlineWidth,
		GlyphSizer& sizer
		);

	unsigned int* WriteGlyphNoStroker(
		int unicode,
		int size,
		union Pixel32 col,
		GlyphSizer& sizer
		);

private:
	FT_Library m_library;
	FT_Face m_face;

}; // FTRender

}

#endif // _DRAG2D_FREETYPE_RENDER_H_
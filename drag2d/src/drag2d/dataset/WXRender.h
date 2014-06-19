#ifndef _DRAG2D_WX_RENDER_H_
#define _DRAG2D_WX_RENDER_H_

#include "glyph_util.h"

namespace d2d
{

class WXRender
{
public:
	WXRender();
	~WXRender();

	void LoadFont(const char* filename);

	GlyphSizer GetGlyphSizer(int unicode, int size);

// 	unsigned int* WriteGlyphWithStroker(
// 		int unicode,
// 		int size,
// 		union Pixel32 fontCol,
// 		union Pixel32 outlineCol,
// 		float outlineWidth,
// 		GlyphLayout& layout
// 		);
// 
// 	unsigned int* WriteGlyphNoStroker(
// 		int unicode,
// 		int size,
// 		union Pixel32 col,
// 		GlyphLayout& layout
// 		);

}; // WXRender

}

#endif // _DRAG2D_WX_RENDER_H_
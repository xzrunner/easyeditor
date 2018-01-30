#ifndef _S2LOADER_GLYPH_STYLE_H_
#define _S2LOADER_GLYPH_STYLE_H_

#include <stdint.h>

struct gtxt_glyph_style;

namespace s2loader
{

class GlyphStyle
{
public:
	int      font;
	int      font_size;
	uint32_t font_color;

	int      edge;
	int      edge_size;
	uint32_t edge_color;

public:
	GlyphStyle();
	GlyphStyle(const gtxt_glyph_style* gs);

	bool operator == (const GlyphStyle& style) const;

}; // GlyphStyle

}

#endif // _S2LOADER_GLYPH_STYLE_H_

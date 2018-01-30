#include "s2loader/GlyphStyle.h"

#include <gtxt_glyph.h>

namespace s2loader
{

GlyphStyle::GlyphStyle()
	: font(-1)
	, font_size(-1)
	, font_color(0)
	, edge(-1)
	, edge_size(-1)
	, edge_color(0)
{
}

GlyphStyle::GlyphStyle(const gtxt_glyph_style* gs)
	: font(gs->font)
	, font_size(gs->font_size)
	, font_color(gs->font_color.integer)
	, edge(gs->edge)
	, edge_size(static_cast<int>(gs->edge_size))
	, edge_color(gs->edge_color.integer)
{}

bool GlyphStyle::operator == (const GlyphStyle& style) const
{
	return font       == style.font 
		&& font_size  == style.font_size
		&& font_color == style.font_color
		&& edge       == style.edge
		&& edge_size  == style.edge_size
		&& edge_color == style.edge_color;
}

}
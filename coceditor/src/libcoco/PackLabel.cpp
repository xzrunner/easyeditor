#include "PackLabel.h"

namespace libcoco
{

PackLabel::PackLabel(const d2d::FontSprite* font)
{
	Load(font);
}

void PackLabel::Load(const d2d::FontSprite* font)
{
	m_name = font->name;
	m_font = font->font;

	m_align_hori = font->align_hori;
	m_align_vert = font->align_vert;

	m_color = font->color;

	m_width = font->width;
	m_height = font->height;

	m_edge = font->has_edge;
}

}
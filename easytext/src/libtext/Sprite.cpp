#include "Sprite.h"
#include "PropertySetting.h"

namespace etext
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_width = 100;
	m_height = 20;

	m_font_size = 16;

	m_has_edge = false;

	m_color.set(1, 1, 1);

	m_align_hori = HAT_LEFT;
	m_align_vert = VAT_TOP;
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->Retain();

	m_width = sprite.m_width;
	m_height = sprite.m_height;

	m_font_size = sprite.m_font_size;

	m_has_edge = sprite.m_has_edge;

	m_color = sprite.m_color;

	m_align_hori = sprite.m_align_hori;
	m_align_vert = sprite.m_align_vert;
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();

	m_width = symbol->m_width;
	m_height = symbol->m_height;

	m_font_size = symbol->m_font_size;

	m_has_edge = symbol->m_has_edge;

	m_color = d2d::transColor(symbol->m_color, d2d::PT_ARGB);

	m_align_hori = symbol->m_align_hori;
	m_align_vert = symbol->m_align_vert;

	BuildBounding();	
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	d2d::ISprite::SetSymbol(&m_symbol, symbol);
}

d2d::IPropertySetting* Sprite::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

}
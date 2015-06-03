#include "Symbol.h"
#include "Shadow.h"
#include "config.h"

namespace eshadow
{

Symbol::Symbol()
{
	m_shadow = new Shadow(SOFT_SHADOW_RADIUS);
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
	m_shadow = new Shadow(SOFT_SHADOW_RADIUS);
}

Symbol::~Symbol()
{
	if (m_shadow) {
		m_shadow->Release();
	}
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this);
}

void Symbol::reloadTexture() const
{
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite) const
{
	if (m_shadow) {
		m_shadow->Draw(mt);
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
	if (m_shadow) {
		return m_shadow->GetRegion();
	} else {
		return d2d::Rect(200, 200);
	}
}

void Symbol::loadResources()
{
}

}
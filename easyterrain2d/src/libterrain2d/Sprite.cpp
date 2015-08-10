#include "Sprite.h"

namespace eterrain2d
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& s)
	: d2d::ISprite(s)
{
	m_symbol = s.m_symbol->Clone();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
//	rotate(-m_symbol->GetOceanAngle());
	m_symbol->Retain();
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
	return new Sprite(*this);
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	`		¡¤¡¤                 65SetSymbol<Symbol>(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	d2d::ISprite::Load(val);
//	rotate(-m_symbol->GetOceanAngle());
}

void Sprite::BuildBounding()
{
	if (!m_bounding) 
		m_bounding = d2d::BVFactory::createBV(d2d::e_obb);

	d2d::Rect rect = m_symbol->GetSize();
	if (m_offset.x == 0 && m_offset.y == 0)
		m_offset.set(rect.xCenter(), rect.yCenter());
	rect.scale(m_scale.x, m_scale.y);
	rect.shear(m_shear.x, m_shear.y);
	m_bounding->initFromRect(rect);
	m_bounding->SetTransform(m_pos, m_offset, m_angle);
}

}
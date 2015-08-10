#include "Sprite.h"
#include "FileIO.h"
#include "Shape.h"

namespace emesh
{

Sprite::Sprite()
	: m_symbol(NULL)
{
//	m_speed.set(0, -0.01f);
}

Sprite::Sprite(const Sprite& s)
	: d2d::ISprite(s)
	, m_speed(s.m_speed)
{
	m_symbol = s.m_symbol->Clone();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
//	m_speed.set(0, -0.01f);

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
	d2d::ISprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	ISprite::Load(val);
	m_speed.x = val["speed"]["x"].asDouble();
	m_speed.y = val["speed"]["y"].asDouble();

// 	// ¼æÈÝÀÏÊý¾Ý
// 	if (!val["left nodes"].isNull()) {
// 		m_symbol->getShape()->Load(val);
// 	}
}

void Sprite::Store(Json::Value& val) const
{
	ISprite::Store(val);
	val["speed"]["x"] = m_speed.x;
	val["speed"]["y"] = m_speed.y;

//	m_symbol->getShape()->Store(val);
}

void Sprite::BuildBounding()
{
	if (!m_bounding) 
		m_bounding = d2d::BVFactory::createBV(d2d::e_obb);
	const d2d::ISymbol& symbol = GetSymbol();

	d2d::Rect rect = m_symbol->GetSize();
	if (m_offset.x == 0 && m_offset.y == 0)
		m_offset.set(rect.xCenter(), rect.yCenter());
	rect.scale(m_scale.x, m_scale.y);
	rect.shear(m_shear.x, m_shear.y);
	m_bounding->initFromRect(rect);
	m_bounding->SetTransform(m_pos, m_offset, m_angle);
}

void Sprite::SetTween(Sprite* begin, Sprite* end, float process)
{
	getShape()->SetTween(begin->getShape(), end->getShape(), process);
}

}
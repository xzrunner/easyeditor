#include "Sprite.h"
#include "FileIO.h"
#include "Shape.h"

#include <ee/BBFactory.h>
#include <ee/BoundingBox.h>

namespace emesh
{

Sprite::Sprite()
	: m_symbol(NULL)
{
//	m_speed.set(0, -0.01f);
}

Sprite::Sprite(const Sprite& s)
	: ee::Sprite(s)
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

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	ee::Sprite::Load(val);
	m_speed.x = static_cast<float>(val["speed"]["x"].asDouble());
	m_speed.y = static_cast<float>(val["speed"]["y"].asDouble());

// 	// ¼æÈÝÀÏÊý¾Ý
// 	if (!val["left nodes"].isNull()) {
// 		m_symbol->getShape()->Load(val);
// 	}
}

void Sprite::Store(Json::Value& val) const
{
	ee::Sprite::Store(val);
	val["speed"]["x"] = m_speed.x;
	val["speed"]["y"] = m_speed.y;

//	m_symbol->getShape()->Store(val);
}

void Sprite::SetTween(Sprite* begin, Sprite* end, float process)
{
	getShape()->SetTween(begin->getShape(), end->getShape(), process);
}

}
#include "Sprite.h"
#include "FileIO.h"

namespace emesh
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_speed.set(0, -0.01f);
}

Sprite::Sprite(const Sprite& s)
	: d2d::ISprite(s)
	, m_speed(s.m_speed)
{
	m_symbol = s.m_symbol->clone();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_speed.set(0, -0.01f);

	m_symbol->retain();
	buildBounding();
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->release();
	}
}

Sprite* Sprite::clone() const
{
	return new Sprite(*this);
}

const Symbol& Sprite::getSymbol() const
{
	return *m_symbol;
}

void Sprite::setSymbol(d2d::ISymbol* symbol)
{
	Symbol* mesh = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != mesh)
	{
		if (m_symbol) {
			m_symbol->release();
		}
		symbol->retain();

		m_symbol = mesh;
		buildBounding();
	}
}

void Sprite::loadBodyFromFile()
{
}

void Sprite::load(const Json::Value& val)
{
	ISprite::load(val);
	m_speed.x = val["speed"]["x"].asDouble();
	m_speed.y = val["speed"]["y"].asDouble();
}

void Sprite::store(Json::Value& val) const
{
	ISprite::store(val);
	val["speed"]["x"] = m_speed.x;
	val["speed"]["y"] = m_speed.y;
}

}
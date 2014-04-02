#include "Sprite.h"
#include "FileIO.h"

namespace emesh
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& s)
	: d2d::ISprite(s)
{
	m_symbol = s.m_symbol->clone();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
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
	d2d::ISprite::load(val);
	FileIO::loadTransform(val["transform"], m_symbol->getMesh());
}

void Sprite::store(Json::Value& val) const
{
	d2d::ISprite::store(val);
	FileIO::storeTransform(m_symbol->getMesh(), val["transform"]);
}

}
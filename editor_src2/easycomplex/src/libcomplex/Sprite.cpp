#include "Sprite.h"

namespace ecomplex
{
	Sprite::Sprite()
		: m_symbol(NULL)
	{
	}

	Sprite::Sprite(const Sprite& sprite)
		: ISprite(sprite)
		, m_symbol(sprite.m_symbol)
	{
	}

	Sprite::Sprite(Symbol* symbol)
		: m_symbol(symbol)
	{
		buildBounding();	
	}

	Sprite::~Sprite()
	{
	}

	Sprite* Sprite::clone() const
	{
		Sprite* sprite = new Sprite(*this);
		d2d::SpriteFactory::Instance()->insert(sprite);
		return sprite;
	}

	const Symbol& Sprite::getSymbol() const
	{
		return *m_symbol;
	}

	void Sprite::setSymbol(d2d::ISymbol* symbol)
	{
		Symbol* complex = dynamic_cast<Symbol*>(symbol);
		if (complex) m_symbol = complex;
	}

	void Sprite::loadBodyFromFile()
	{
	}
}
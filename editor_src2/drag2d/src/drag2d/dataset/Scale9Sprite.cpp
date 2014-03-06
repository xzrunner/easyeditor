#include "Scale9Sprite.h"
#include "SpriteFactory.h"

#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

namespace d2d
{

Scale9Sprite::Scale9Sprite()
	: m_symbol(NULL)
{
	width = height = 0;
}

Scale9Sprite::Scale9Sprite(const Scale9Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	width = m_symbol->getSize().xLength();
	height = m_symbol->getSize().yLength();
}

Scale9Sprite::Scale9Sprite(Scale9Symbol* symbol)
	: m_symbol(symbol)
{
	width = m_symbol->getSize().xLength();
	height = m_symbol->getSize().yLength();
	buildBounding();
}

Scale9Sprite::~Scale9Sprite()
{
}

Scale9Sprite* Scale9Sprite::clone() const
{
	Scale9Sprite* sprite = new Scale9Sprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Scale9Symbol& Scale9Sprite::getSymbol() const
{
	return *m_symbol;
}

void Scale9Sprite::setSymbol(ISymbol* symbol)
{
	Scale9Symbol* anim = dynamic_cast<Scale9Symbol*>(symbol);
	if (anim) m_symbol = anim;
}

void Scale9Sprite::loadBodyFromFile()
{
}

} // d2d
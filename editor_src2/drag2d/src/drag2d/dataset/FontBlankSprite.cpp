#include "FontBlankSprite.h"
#include "SpriteFactory.h"

#include "common/Rect.h"
#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

namespace d2d
{

FontBlankSprite::FontBlankSprite()
	: m_symbol(NULL)
{
}

FontBlankSprite::FontBlankSprite(const FontBlankSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

FontBlankSprite::FontBlankSprite(FontBlankSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

FontBlankSprite::~FontBlankSprite()
{
}

FontBlankSprite* FontBlankSprite::clone() const
{
	FontBlankSprite* sprite = new FontBlankSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const FontBlankSymbol& FontBlankSprite::getSymbol() const
{
	return *m_symbol;
}

void FontBlankSprite::setSymbol(ISymbol* symbol)
{
	FontBlankSymbol* font = dynamic_cast<FontBlankSymbol*>(symbol);
	if (font) m_symbol = font;
}

void FontBlankSprite::loadBodyFromFile()
{
}

} // d2d
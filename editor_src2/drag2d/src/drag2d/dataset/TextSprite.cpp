#include "TextSprite.h"
#include "SpriteFactory.h"

#include "common/Rect.h"
#include "dataset/Font.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

namespace d2d
{

TextSprite::TextSprite()
	: m_symbol(NULL)
	, m_text("text")
	, m_size(Font::DEFAULT_SIZE)
{
}

TextSprite::TextSprite(const TextSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_text("text")
	, m_size(Font::DEFAULT_SIZE)
{
	m_symbol->retain();
}

TextSprite::TextSprite(FontSymbol* symbol)
	: m_symbol(symbol)
	, m_text("text")
	, m_size(Font::DEFAULT_SIZE)
{
	m_symbol->retain();
	buildBounding();
}

TextSprite::~TextSprite()
{
	m_symbol->release();
}

TextSprite* TextSprite::clone() const
{
	TextSprite* sprite = new TextSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const FontSymbol& TextSprite::getSymbol() const
{
	return *m_symbol;
}

void TextSprite::setSymbol(ISymbol* symbol)
{
	FontSymbol* font = dynamic_cast<FontSymbol*>(symbol);
	if (m_symbol != symbol && font)
	{
		m_symbol->release();
		symbol->retain();

		m_symbol = font;
	}
}

void TextSprite::loadBodyFromFile()
{
}

void TextSprite::setTransform(const Vector& position, float angle)
{
	Vector fixed(position);
	fixed.x -= m_symbol->getSize(this).xLength() * 0.5f;
	fixed.y -= m_symbol->getSize(this).yLength() * 0.5f;

	ISprite::setTransform(fixed, angle);
}

void TextSprite::setText(const std::string& text)
{
	m_text = text;
	buildBounding();
}

void TextSprite::setSize(int size)
{
	m_size = size;
	float scale = (float)size / Font::DEFAULT_SIZE;
	setScale(scale, scale);
}

} // d2d
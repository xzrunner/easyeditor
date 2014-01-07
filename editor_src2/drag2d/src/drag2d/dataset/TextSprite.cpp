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
}

TextSprite::TextSprite(FontSymbol* symbol)
	: m_symbol(symbol)
	, m_text("text")
	, m_size(Font::DEFAULT_SIZE)
{
	buildBounding();
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
	if (font) m_symbol = font;
}

void TextSprite::loadBodyFromFile()
{
}

void TextSprite::setTransform(const Vector& position, float angle)
{
	Vector fixed(position);
	fixed.x -= m_symbol->getWidth(this) * 0.5f;
	fixed.y -= m_symbol->getHeight(this) * 0.5f;

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
	setScale((float)size / Font::DEFAULT_SIZE);
}

void TextSprite::buildBounding()
{
	if (!m_symbol) return;

	const float width = m_symbol->getWidth(this) * m_xScale,
		height = m_symbol->getHeight(this) * m_yScale;

	delete m_bounding;
	m_bounding = BVFactory::createBV(e_aabb);
	Rect rect;
	rect.xMin = 0;
	rect.xMax = width;
	rect.yMin = 0;
	rect.yMax = height;
	rect.translate(m_pos);
	m_bounding->initFromRect(rect);
}
} // d2d
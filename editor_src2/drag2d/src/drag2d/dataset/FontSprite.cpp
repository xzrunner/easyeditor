#include "FontSprite.h"
#include "SpriteFactory.h"

#include "common/Rect.h"
#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

namespace d2d
{

FontSprite::FontSprite()
	: m_symbol(NULL)
{
	font = "";
	color = "0xffffffff";
	align = AT_LEFT;
	size = 16;
	width = 100;
	height = 20;
}

FontSprite::FontSprite(const FontSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	font = sprite.font;
	color = sprite.color;
	align = sprite.align;
	size = sprite.size;
	width = sprite.width;
	height = sprite.height;
}

FontSprite::FontSprite(FontBlankSymbol* symbol)
	: m_symbol(symbol)
{
	font = m_symbol->font;
	color = m_symbol->color;
	if (symbol->align == 0)
		align = AT_LEFT;
	else if (symbol->align == 1)
		align = AT_RIGHT;
	else
		align = AT_CENTER;
	size = symbol->size;
	width = symbol->width;
	height = symbol->height;

	buildBounding();
}

FontSprite::~FontSprite()
{
}

FontSprite* FontSprite::clone() const
{
	FontSprite* sprite = new FontSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const FontBlankSymbol& FontSprite::getSymbol() const
{
	return *m_symbol;
}

void FontSprite::setSymbol(ISymbol* symbol)
{
	FontBlankSymbol* font = dynamic_cast<FontBlankSymbol*>(symbol);
	if (font) m_symbol = font;
}

void FontSprite::loadBodyFromFile()
{
}

void FontSprite::load(const Json::Value& val)
{
	ISprite::load(val);

	// test if is old version
	if (val["font"].isNull())
	{
		font = m_symbol->font;
		color = m_symbol->color;
		align = AlignType((int)m_symbol->align);
		size = m_symbol->size;
		width = m_symbol->width;
		height = m_symbol->height;
	}
	else
	{
		font = val["font"].asString();
		color = val["color"].asString();
		align = AlignType(val["align"].asInt());
		size = val["size"].asInt();
		width = val["label_width"].asInt();
		height = val["label_height"].asInt();
	}

	buildBounding();
}

void FontSprite::store(Json::Value& val)
{
	ISprite::store(val);

	val["font"] = font;
	val["color"] = color;
	val["align"] = align;
	val["size"] = size;
	val["label_width"] = width;
	val["label_height"] = height;
}

void FontSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	m_bounding->combine(Rect(m_pos, width * m_xScale * 0.5f,
		height * m_yScale * 0.5f));
	m_bounding->rotate(m_angle);
}

}
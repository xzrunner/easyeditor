#include "ComplexSprite.h"
#include "SpriteFactory.h"

#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

namespace d2d
{

ComplexSprite::ComplexSprite()
	: m_symbol(NULL)
{
}

ComplexSprite::ComplexSprite(const ComplexSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

ComplexSprite::ComplexSprite(ComplexSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();	
}

ComplexSprite::~ComplexSprite()
{
}

ComplexSprite* ComplexSprite::clone()
{
	ComplexSprite* sprite = new ComplexSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const ComplexSymbol& ComplexSprite::getSymbol() const
{
	return *m_symbol;
}

void ComplexSprite::setSymbol(ISymbol* symbol)
{
	ComplexSymbol* complex = dynamic_cast<ComplexSymbol*>(symbol);
	if (complex) m_symbol = complex;
}

void ComplexSprite::loadBodyFromFile()
{
}

void ComplexSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		Rect rect = m_symbol->m_rect;

		rect.scale(m_xScale, m_yScale);
		rect.translate(m_pos);
		m_bounding->initFromRect(rect);
		m_bounding->rotate(m_angle);
	}
}

} // d2d
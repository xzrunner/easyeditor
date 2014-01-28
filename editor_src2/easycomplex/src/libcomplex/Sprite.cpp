#include "Sprite.h"

namespace complex
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

	void Sprite::buildBounding()
	{
		delete m_bounding;
		m_bounding = d2d::BVFactory::createBV(d2d::e_obb);

		if (m_symbol)
		{
			d2d::Rect rect = m_symbol->m_rect;

			rect.scale(m_xScale, m_yScale);
			rect.translate(m_pos);
			m_bounding->initFromRect(rect);
			m_bounding->rotate(m_angle);
		}
	}
}
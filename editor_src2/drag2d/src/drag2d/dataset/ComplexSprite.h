#pragma once

#include "ISprite.h"

#include "ComplexSymbol.h"

namespace d2d
{
	class ComplexSprite : public ISprite
	{
	public:
		ComplexSprite();
		ComplexSprite(const ComplexSprite& sprite);
		ComplexSprite(ComplexSymbol* symbol);
		virtual ~ComplexSprite();

		//
		// IObject interface
		//
		virtual ComplexSprite* clone() const;

		//
		// ISprite interface
		//
		virtual const ComplexSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

		virtual void buildBounding();

	protected:
		ComplexSymbol* m_symbol;

	}; // ComplexSprite
}


#pragma once

#include "ISprite.h"

#include "ShapeSymbol.h"

namespace d2d
{
	class ShapeSymbol;

	class ShapeSprite : public ISprite
	{
	public:
		ShapeSprite();
		ShapeSprite(const ShapeSprite& sprite);
		ShapeSprite(ShapeSymbol* symbol);
		virtual ~ShapeSprite();

		//
		// ICloneable interface
		//
		virtual ShapeSprite* clone() const;

		//
		// ISprite interface
		//
		virtual const ShapeSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();
		
	private:
		ShapeSymbol* m_symbol;

	}; // ShapeSprite
}


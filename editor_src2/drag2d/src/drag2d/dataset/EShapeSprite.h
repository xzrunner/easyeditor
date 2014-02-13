#pragma once

#include "ISprite.h"

#include "EShapeSymbol.h"

namespace d2d
{
	class EShapeSprite : public ISprite
	{
	public:
		EShapeSprite();
		EShapeSprite(const EShapeSprite& sprite);
		EShapeSprite(EShapeSymbol* symbol);
		virtual ~EShapeSprite();

		//
		// IObject interface
		//
		virtual EShapeSprite* clone() const;

		//
		// ISprite interface
		//
		virtual const EShapeSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

	private:
		EShapeSymbol* m_symbol;

	}; // EShapeSprite
}


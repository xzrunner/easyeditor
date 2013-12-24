#pragma once

#include "ISprite.h"

#include "CombinationSymbol.h"

namespace d2d
{
	class CombinationSymbol;

	class CombinationSprite : public ISprite
	{
	public:
		CombinationSprite();
		CombinationSprite(const CombinationSprite& sprite);
		CombinationSprite(CombinationSymbol* symbol);
		virtual ~CombinationSprite();

		//
		// IObject interface
		//
		virtual CombinationSprite* clone();

		//
		// ISprite interface
		//
		virtual const CombinationSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

		virtual void buildBounding();

	protected:
		CombinationSymbol* m_symbol;

	}; // CombinationSprite
}


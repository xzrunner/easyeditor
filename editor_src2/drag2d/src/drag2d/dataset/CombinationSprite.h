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
		virtual CombinationSprite* clone() const;

		//
		// ISprite interface
		//
		virtual const CombinationSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

	protected:
		CombinationSymbol* m_symbol;

	}; // CombinationSprite
}


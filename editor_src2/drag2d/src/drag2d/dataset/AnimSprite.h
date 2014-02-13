#pragma once

#include "ISprite.h"

#include "AnimSymbol.h"

namespace d2d
{
	class AnimSprite : public ISprite
	{
	public:
		AnimSprite();
		AnimSprite(const AnimSprite& sprite);
		AnimSprite(AnimSymbol* symbol);
		virtual ~AnimSprite();

		//
		// IObject interface
		//
		virtual AnimSprite* clone() const;

		//
		// ISprite interface
		//
		virtual const AnimSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

	protected:
		AnimSymbol* m_symbol;

	}; // AnimSprite
}


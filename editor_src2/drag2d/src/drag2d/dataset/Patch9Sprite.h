#pragma once

#include "ISprite.h"

#include "Patch9Symbol.h"

namespace d2d
{
	class Patch9Sprite : public ISprite
	{
	public:
		Patch9Sprite();
		Patch9Sprite(const Patch9Sprite& sprite);
		Patch9Sprite(Patch9Symbol* symbol);
		virtual ~Patch9Sprite();

		//
		// IObject interface
		//
		virtual Patch9Sprite* clone() const;

		//
		// ISprite interface
		//
		virtual const Patch9Symbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

	public:
		float width, height;

	protected:
		Patch9Symbol* m_symbol;

	}; // Patch9Sprite
}


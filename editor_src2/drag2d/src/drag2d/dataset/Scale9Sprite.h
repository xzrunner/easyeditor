#pragma once

#include "ISprite.h"

#include "Scale9Symbol.h"

namespace d2d
{
	class Scale9Sprite : public ISprite
	{
	public:
		Scale9Sprite();
		Scale9Sprite(const Scale9Sprite& sprite);
		Scale9Sprite(Scale9Symbol* symbol);
		virtual ~Scale9Sprite();

		//
		// IObject interface
		//
		virtual Scale9Sprite* clone() const;

		//
		// ISprite interface
		//
		virtual const Scale9Symbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

	public:
		float width, height;

	protected:
		Scale9Symbol* m_symbol;

	}; // Scale9Sprite
}


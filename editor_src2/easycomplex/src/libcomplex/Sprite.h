#ifndef COMPLEX_SPRITE_H
#define COMPLEX_SPRITE_H

#include "Symbol.h"

namespace complex
{
	class Sprite : public d2d::ISprite
	{
	public:
		Sprite();
		Sprite(const Sprite& sprite);
		Sprite(Symbol* symbol);
		virtual ~Sprite();

		//
		// IObject interface
		//
		virtual Sprite* clone() const;

		//
		// ISprite interface
		//
		virtual const Symbol& getSymbol() const;
		virtual void setSymbol(d2d::ISymbol* symbol);
		virtual void loadBodyFromFile();

		virtual void buildBounding();

	protected:
		Symbol* m_symbol;

	}; // Sprite
}

#endif // COMPLEX_SPRITE_H
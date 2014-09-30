#ifndef _LIBSKETCH_SPRITE_H_
#define _LIBSKETCH_SPRITE_H_

#include "Symbol.h"

namespace libsketch
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(Symbol* symbol);
	Sprite(const Sprite& sprite);
	virtual ~Sprite();

	//
	// IObject interface
	//
	virtual Sprite* clone() const;

	//
	// d2d::ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);
	virtual void loadBodyFromFile();

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _LIBSKETCH_SPRITE_H_
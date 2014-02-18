#ifndef _ANIMATION_SPRITE_H_
#define _ANIMATION_SPRITE_H_

#include <drag2d.h>

#include "Symbol.h"

namespace anim
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

protected:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _ANIMATION_SPRITE_H_
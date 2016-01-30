#ifndef _LIBSHAPE_SPRITE_H_
#define _LIBSHAPE_SPRITE_H_

#include "Symbol.h"

namespace libshape
{

class Sprite : public d2d::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// IObject interface
	//
	virtual Sprite* Clone() const;

	//
	// d2d::Sprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::Symbol* symbol);

	static d2d::Sprite* Create(d2d::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _LIBSHAPE_SPRITE_H_
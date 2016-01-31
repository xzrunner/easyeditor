#ifndef _LIBSHAPE_SPRITE_H_
#define _LIBSHAPE_SPRITE_H_

#include "Symbol.h"

namespace libshape
{

class Sprite : public ee::Sprite
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
	// ee::Sprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _LIBSHAPE_SPRITE_H_
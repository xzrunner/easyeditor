#ifndef _LIBANIS_SPRITE_H_
#define _LIBANIS_SPRITE_H_

#include <drag2d.h>

#include "Symbol.h"

namespace libanis
{

class Sprite : public d2d::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// Cloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(int version) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::Symbol* symbol);

	static d2d::Sprite* Create(d2d::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

	int m_idx;

}; // Sprite

}

#endif // _LIBANIS_SPRITE_H_
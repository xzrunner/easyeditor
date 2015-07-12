#ifndef _LIBANIS_SPRITE_H_
#define _LIBANIS_SPRITE_H_

#include <drag2d.h>

#include "Symbol.h"

namespace libanis
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// ICloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// ISprite interface
	//
	virtual bool Update(int version) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::ISymbol* symbol);

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

	int m_idx;

}; // Sprite

}

#endif // _LIBANIS_SPRITE_H_
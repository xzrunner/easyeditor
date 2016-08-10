#ifndef _EASYCOMPLEX_SPRITE_H_
#define _EASYCOMPLEX_SPRITE_H_

#include <ee/Sprite.h>

#include "Symbol.h"

namespace ecomplex
{

class Sprite : public ee::Sprite
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
	virtual bool Update(const s2::RenderParams& params, float dt);
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

protected:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _EASYCOMPLEX_SPRITE_H_
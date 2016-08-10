#ifndef _EASYTRAIL_SPRITE_H_
#define _EASYTRAIL_SPRITE_H_

#include "Symbol.h"
#include "TrailNode.h"

#include <ee/Sprite.h>

namespace etrail
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
	virtual bool Update(float dt);
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	void Draw(const s2::RenderParams& params) const;

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

protected:
	Symbol* m_symbol;

private:
	t2d_emitter* m_et;

	mutable RenderParams m_rp;

}; // Sprite

}

#endif // _EASYTRAIL_SPRITE_H_
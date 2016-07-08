#ifndef _EASYUI_WINDOW_SPRITE_H_
#define _EASYUI_WINDOW_SPRITE_H_

#include "Symbol.h"

#include <ee/Sprite.h>

namespace eui
{
namespace window
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

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

}; // Sprite

}
}

#endif // _EASYUI_WINDOW_SPRITE_H_
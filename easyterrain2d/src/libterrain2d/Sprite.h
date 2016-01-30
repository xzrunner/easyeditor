#ifndef _EASYTERRAIN2D_SPRITE_H_
#define _EASYTERRAIN2D_SPRITE_H_

#include "Symbol.h"

namespace eterrain2d
{

class Sprite : public d2d::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& s);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// IObject interface
	//
	virtual Sprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(int version) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::Symbol* symbol);
	virtual void Load(const Json::Value& val);

	static d2d::Sprite* Create(d2d::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _EASYTERRAIN2D_SPRITE_H_
#ifndef _EASYTERRAIN2D_SPRITE_H_
#define _EASYTERRAIN2D_SPRITE_H_

#include "Symbol.h"

namespace eterrain2d
{

class Sprite : public d2d::ISprite
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
	// ISprite interface
	//
	virtual bool Update(int version) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::ISymbol* symbol);
	virtual void Load(const Json::Value& val);
	virtual void BuildBounding();

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _EASYTERRAIN2D_SPRITE_H_
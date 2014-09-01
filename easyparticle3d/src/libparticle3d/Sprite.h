#ifndef _EASYPARTICLE3D_SPRITE_H_
#define _EASYPARTICLE3D_SPRITE_H_

#include "Symbol.h"

namespace eparticle3d
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
	virtual Sprite* clone() const;

	//
	// ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);
	virtual void loadBodyFromFile();

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

protected:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _EASYPARTICLE3D_SPRITE_H_
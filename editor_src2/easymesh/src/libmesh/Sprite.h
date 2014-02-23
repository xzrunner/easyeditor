#ifndef _EASYMESH_SPRITE_H_
#define _EASYMESH_SPRITE_H_

#include <drag2d.h>

#include "Symbol.h"

namespace emesh
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(const Sprite& s);
	Sprite(Symbol* symbol);

	//
	// IObject interface
	//
	virtual Sprite* clone() const;

	//
	// ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);
	virtual void loadBodyFromFile();

	Mesh* getMesh() { 
		return m_symbol ? m_symbol->getMesh() : NULL; 
	}

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _EASYMESH_SPRITE_H_
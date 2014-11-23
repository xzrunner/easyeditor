#ifndef _DRAG2D_SCRIPTS_SPRITE_H_
#define _DRAG2D_SCRIPTS_SPRITE_H_

#include "ISprite.h"
#include "ScriptsSymbol.h"

namespace d2d
{

class ScriptsSprite : public ISprite
{
public:
	ScriptsSprite();
	ScriptsSprite(ScriptsSymbol* symbol);
	virtual ~ScriptsSprite();

	//
	// ICloneable interface
	//
	virtual ScriptsSprite* clone() const;

	//
	// ISprite interface
	//
	virtual const ScriptsSymbol& getSymbol() const;
	virtual void setSymbol(ISymbol* symbol);

private:
	ScriptsSymbol* m_symbol;

}; // ScriptsSprite 

}

#endif // _DRAG2D_SCRIPTS_SPRITE_H_
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
	virtual ScriptsSprite* Clone() const;

	//
	// ISprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const ScriptsSymbol& GetSymbol() const;
	virtual void SetSymbol(ISymbol* symbol);

private:
	ScriptsSymbol* m_symbol;

}; // ScriptsSprite 

}

#endif // _DRAG2D_SCRIPTS_SPRITE_H_
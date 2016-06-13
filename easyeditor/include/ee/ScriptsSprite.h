#ifndef _EASYEDITOR_SCRIPTS_SPRITE_H_
#define _EASYEDITOR_SCRIPTS_SPRITE_H_

#include "Sprite.h"
#include "ScriptsSymbol.h"

namespace ee
{

class ScriptsSprite : public Sprite
{
public:
	ScriptsSprite();
	ScriptsSprite(ScriptsSymbol* symbol);
	virtual ~ScriptsSprite();

	//
	// Cloneable interface
	//
	virtual ScriptsSprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(float dt, const sm::mat4& mat) { return false; }
	virtual const ScriptsSymbol& GetSymbol() const;
	virtual void SetSymbol(Symbol* symbol);

private:
	ScriptsSymbol* m_symbol;

}; // ScriptsSprite 

}

#endif // _EASYEDITOR_SCRIPTS_SPRITE_H_
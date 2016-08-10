#ifndef _EASYEDITOR_DUMMY_SPRITE_H_
#define _EASYEDITOR_DUMMY_SPRITE_H_

#include "Sprite.h"
#include "Symbol.h"

namespace ee
{

class Symbol;

class DummySprite : public Sprite
{
public:
	DummySprite();
	DummySprite(Symbol* symbol);
	~DummySprite();

	//
	// IObject interface
	//
	virtual DummySprite* Clone() const { return NULL; }

	//
	// Sprite interface
	//
	virtual const Symbol& GetSymbol() const { return *m_symbol; }
	virtual void SetSymbol(Symbol* symbol) { m_symbol = symbol; }

private:
	Symbol* m_symbol;

}; // DummySprite

}

#endif // _EASYEDITOR_DUMMY_SPRITE_H_
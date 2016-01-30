#ifndef _EASYEDITOR_NULL_SPRITE_H_
#define _EASYEDITOR_NULL_SPRITE_H_

#include "Sprite.h"

namespace ee
{

class Symbol;

class NullSprite : public Sprite
{
public:
	NullSprite(Symbol* symbol) 
		: m_symbol(symbol) {
			m_symbol->Retain();
	}
	~NullSprite() {
		if (m_symbol) {
			m_symbol->Release();
		}
	}

	//
	// IObject interface
	//
	virtual NullSprite* Clone() const { return NULL; }

	//
	// Sprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const Symbol& GetSymbol() const { return *m_symbol; }
	virtual void SetSymbol(Symbol* symbol) { m_symbol = symbol; }

private:
	Symbol* m_symbol;

}; // NullSprite

}

#endif // _EASYEDITOR_NULL_SPRITE_H_
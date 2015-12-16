#ifndef _EASYTEMPLATE_SYMBOL_H_
#define _EASYTEMPLATE_SYMBOL_H_

#include <drag2d.h>

namespace etemplate
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::ISprite* spr = NULL, const d2d::ISprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const;

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

}; // Symbol

}

#endif // _EASYTEMPLATE_SYMBOL_H_
#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <drag2d.h>

#include "Scale9Data.h"

namespace escale9
{

class Symbol : public d2d::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& symbol);

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return new Symbol(*this); }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

	void ResizeScale9(float width, float height);
	void ComposeFromSprites(d2d::Sprite* sprites[3][3], float width, float height);

	const Scale9Data& GetScale9Data() const { return m_data; }

	static d2d::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	Scale9Data m_data;

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_
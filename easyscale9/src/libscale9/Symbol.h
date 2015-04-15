#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <drag2d.h>

#include "Scale9Data.h"

namespace escale9
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& symbol);

	//
	// ICloneable interface
	//
	virtual Symbol* clone() const { return new Symbol(*this); }

	//
	// ISymbol interfaces
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	void ResizeScale9(float width, float height);
	void ComposeFromSprites(d2d::ISprite* sprites[3][3], float width, float height);

	const Scale9Data& GetScale9Data() const { return m_data; }

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void loadResources();

private:
	Scale9Data m_data;

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_
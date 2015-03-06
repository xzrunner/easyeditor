#ifndef _EASYTEXTURE_SYMBOL_H_
#define _EASYTEXTURE_SYMBOL_H_

#include <drag2d.h>

namespace etexture
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* clone() const;

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

	const std::vector<d2d::IShape*>& GetAllShapes() const { return m_shapes; }

	static d2d::ISymbol* Create() { return new Symbol(); }
	
protected:
	virtual void loadResources();

private:
	std::vector<d2d::IShape*> m_shapes;

	friend class SymbolContainer;

}; // Symbol

}

#endif // _EASYTEXTURE_SYMBOL_H_
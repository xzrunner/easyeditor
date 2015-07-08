#ifndef _LIBSHAPE_SYMBOL_H_
#define _LIBSHAPE_SYMBOL_H_

#include <drag2d.h>

#include "ShapeType.h"

namespace libshape
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& symbol);
	virtual ~Symbol();

	//
	// IObject interface
	//	
	virtual Symbol* Clone() const;

	//
	// ISymbol interface
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const;

	void Traverse(d2d::IVisitor& visitor) const;
	bool Add(d2d::IShape* shape);
	bool Remove(d2d::IShape* shape);
	bool Clear();

	void SetBG(d2d::ISymbol* bg);
	const d2d::ISymbol* GetBG() const { return m_bg; }

	void StoreToFile(const char* filename) const;

	ShapeType GetShapeType() const;

	static d2d::ISymbol* Create() { return new Symbol(); }

	// todo
	const std::vector<d2d::IShape*>& GetShapes() const { return m_shapes; }

protected:
	virtual void LoadResources();

private:
	void LoadBGOutline(d2d::ISymbol* bg);
	void LoadBGTriStrip(d2d::ISymbol* bg);

private:
	d2d::ISymbol* m_bg;
	std::vector<d2d::IShape*> m_bg_outline;
	std::vector<std::vector<d2d::Vector> > m_bg_tri_strips;

	std::vector<d2d::IShape*> m_shapes;

}; // Symbol

}

#endif // _LIBSHAPE_SYMBOL_H_
#ifndef _LIBSHAPE_SYMBOL_H_
#define _LIBSHAPE_SYMBOL_H_

#include <drag2d.h>

#include "ShapeType.h"

namespace libshape
{

class Symbol : public d2d::Symbol
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
	// Symbol interface
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

	void Traverse(d2d::Visitor& visitor) const;
	bool Add(d2d::Shape* shape);
	bool Remove(d2d::Shape* shape);
	bool Clear();

	void SetBG(d2d::Symbol* bg);
	const d2d::Symbol* GetBG() const { return m_bg; }

	void StoreToFile(const char* filename) const;

	ShapeType GetShapeType() const;

	static d2d::Symbol* Create() { return new Symbol(); }

	// todo
	const std::vector<d2d::Shape*>& GetShapes() const { return m_shapes; }

protected:
	virtual void LoadResources();

private:
	void LoadBGOutline(d2d::Symbol* bg);
	void LoadBGTriStrip(d2d::Symbol* bg);

private:
	d2d::Symbol* m_bg;
	std::vector<d2d::Shape*> m_bg_outline;
	std::vector<std::vector<d2d::Vector> > m_bg_tri_strips;

	std::vector<d2d::Shape*> m_shapes;

}; // Symbol

}

#endif // _LIBSHAPE_SYMBOL_H_
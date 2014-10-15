#ifndef _LIBSKETCH_SYMBOL_H_
#define _LIBSKETCH_SYMBOL_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// IObject interface
	//	
	virtual Symbol* clone() const;

	//
	// ISymbol interface
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	static d2d::ISymbol* Create() { return new Symbol(); }

	void SetModel(e3d::IModel* model);
	void SetAABB(const e3d::Cube& aabb) { m_aabb = aabb; }

protected:
	virtual void loadResources();

private:
	e3d::IModel* m_model;

	e3d::Cube m_aabb;

}; // Symbol

}

#endif // _LIBSKETCH_SYMBOL_H_
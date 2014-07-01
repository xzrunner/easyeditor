#ifndef _EASYMESH_SYMBOL_H_
#define _EASYMESH_SYMBOL_H_

#include <drag2d.h>

namespace emesh
{

class Mesh;
class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	Symbol(d2d::Image* image);
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* clone() const;

	//
	// ISymbol interfaces
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::Screen& scr,
		const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	//
	// ListItem interface
	//
	virtual void refresh();

	const Mesh* getMesh() const { return m_mesh; }
	Mesh* getMesh() { return m_mesh; }

	const d2d::Image* getImage() const { return m_image; }
	d2d::Image* getImage() { return m_image; }

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void loadResources();

private:
	d2d::Image* m_image;

	Mesh* m_mesh;

	friend class FileIO;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_
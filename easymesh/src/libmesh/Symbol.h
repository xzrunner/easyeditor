#ifndef _EASYMESH_SYMBOL_H_
#define _EASYMESH_SYMBOL_H_

#include <drag2d.h>

namespace emesh
{

class Shape;
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
	virtual void draw(const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	//
	// ListItem interface
	//
	virtual void refresh();

 	const Shape* getShape() const { return m_shape; }
 	Shape* getShape() { return m_shape; }
	void SetShape(Shape* shape);

	const d2d::Image* getImage() const { return m_image; }
	d2d::Image* getImage() { return m_image; }

	const wxString& GetImagePath() const;
	void LoadImage(const wxString& filepath);

	void SetPause(bool pause) { m_pause = pause; }

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void loadResources();

private:
	d2d::Image* m_image;

	Shape* m_shape;

	bool m_pause;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_
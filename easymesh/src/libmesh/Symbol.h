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
	virtual Symbol* Clone() const;

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::ISprite* spr = NULL, const d2d::ISprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const {
		return m_region;
	}

 	const Shape* getShape() const { return m_shape; }
 	Shape* getShape() { return m_shape; }
	void SetShape(Shape* shape);

	const d2d::Image* getImage() const { return m_image; }
	d2d::Image* getImage() { return m_image; }

	std::string GetImagePath() const;
	void LoadImage(const std::string& filepath);

	void SetPause(bool pause) { m_pause = pause; }

	void CreateShape();

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void InitBounding();

private:
	d2d::Image* m_image;

	Shape* m_shape;

	bool m_pause;

	d2d::Rect m_region;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_
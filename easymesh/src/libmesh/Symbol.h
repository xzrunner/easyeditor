#ifndef _EASYMESH_SYMBOL_H_
#define _EASYMESH_SYMBOL_H_

#include <ee/Symbol.h>
#include <ee/Rect.h>

namespace ee { class Image; }

namespace emesh
{

class Shape;
class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	Symbol(ee::Image* image);
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const;

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const ee::SpriteTrans& trans, const ee::Sprite* spr = NULL, 
		const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const {
		return m_region;
	}

 	const Shape* getShape() const { return m_shape; }
 	Shape* getShape() { return m_shape; }
	void SetShape(Shape* shape);

	const ee::Image* getImage() const { return m_image; }
	ee::Image* getImage() { return m_image; }

	std::string GetImagePath() const;
	void LoadImage(const std::string& filepath);

	void SetPause(bool pause) { m_pause = pause; }

	void CreateShape();

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void InitBounding();

private:
	ee::Image* m_image;

	Shape* m_shape;

	bool m_pause;

	ee::Rect m_region;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_
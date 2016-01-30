#ifndef _LIBSHAPE_RECT_SHAPE_H_
#define _LIBSHAPE_RECT_SHAPE_H_

#include <drag2d.h>

namespace libshape
{

class RectShape : public d2d::Shape
{
public:
	RectShape();
	RectShape(const RectShape& rect);
	RectShape(const d2d::Vector& p0, const d2d::Vector& p1);
	RectShape(const d2d::Vector& center, float hWidth, float hHeight);

	//
	// IObject interface
	//
	virtual RectShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "rect"; }
	virtual bool IsContain(const d2d::Vector& pos) const;
	virtual bool IsIntersect(const d2d::Rect& rect) const;
	virtual void Translate(const d2d::Vector& offset);
	virtual const d2d::Rect& GetRect() const { return m_rect; }
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans()) const;
	virtual d2d::IPropertySetting* CreatePropertySetting(d2d::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

public:
	d2d::Rect m_rect;

private:
	d2d::ShapeStyle m_style;

}; // RectShape

}

#endif // _LIBSHAPE_RECT_SHAPE_H_
#ifndef _EASYSHAPE_RECT_SHAPE_H_
#define _EASYSHAPE_RECT_SHAPE_H_

#include <ee/Shape.h>
#include <ee/Rect.h>

namespace eshape
{

class RectShape : public ee::Shape
{
public:
	RectShape();
	RectShape(const RectShape& rect);
	RectShape(const ee::Vector& p0, const ee::Vector& p1);
	RectShape(const ee::Vector& center, float hWidth, float hHeight);

	//
	// IObject interface
	//
	virtual RectShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "rect"; }
	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;
	virtual void Translate(const ee::Vector& offset);
	virtual const ee::Rect& GetRect() const { return m_rect; }
	virtual void Draw(const ee::Matrix& mt, const ee::ColorTrans& color = ee::ColorTrans()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

public:
	ee::Rect m_rect;

}; // RectShape

}

#endif // _EASYSHAPE_RECT_SHAPE_H_
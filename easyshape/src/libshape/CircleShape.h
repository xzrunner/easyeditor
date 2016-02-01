#ifndef _EASYSHAPE_CIRCLE_SHAPE_H_
#define _EASYSHAPE_CIRCLE_SHAPE_H_

#include <ee/Shape.h>
#include <ee/Vector.h>
#include <ee/Rect.h>

namespace eshape
{

class CircleShape : public ee::Shape
{
public:
	CircleShape();
	CircleShape(const CircleShape& circle);
	CircleShape(const ee::Vector& center, float radius);

	//
	// IObject interface
	//
	virtual CircleShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "circle"; }
	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;
	virtual void Translate(const ee::Vector& offset);
	virtual const ee::Rect& GetRect() const { return m_rect; }
	virtual void Draw(const ee::Matrix& mt,
		const ee::ColorTrans& color = ee::ColorTrans()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

public:
	ee::Vector center;

	float radius;

private:
	ee::Rect m_rect;

}; // CircleShape

}

#endif // _EASYSHAPE_CIRCLE_SHAPE_H_
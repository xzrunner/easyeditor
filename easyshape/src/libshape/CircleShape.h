#ifndef _LIBSHAPE_CIRCLE_SHAPE_H_
#define _LIBSHAPE_CIRCLE_SHAPE_H_

#include <drag2d.h>

namespace libshape
{

class CircleShape : public d2d::IShape
{
public:
	CircleShape();
	CircleShape(const CircleShape& circle);
	CircleShape(const d2d::Vector& center, float radius);

	//
	// IObject interface
	//
	virtual CircleShape* Clone() const;

	//
	// IShape interface
	//
	virtual const char* GetShapeDesc() const { return "circle"; }
	virtual bool isContain(const d2d::Vector& pos) const;
	virtual bool isIntersect(const d2d::Rect& rect) const;
	virtual void Translate(const d2d::Vector& offset);
	virtual const d2d::Rect& getRect() const { return m_rect; }
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;
	virtual d2d::IPropertySetting* createPropertySetting(d2d::EditPanel* editPanel);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

public:
	d2d::Vector center;

	float radius;

private:
	d2d::Rect m_rect;

}; // CircleShape

}

#endif // _LIBSHAPE_CIRCLE_SHAPE_H_
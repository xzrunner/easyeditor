#ifndef _LIBSHAPE_POINT_SHAPE_H_
#define _LIBSHAPE_POINT_SHAPE_H_

#include <drag2d.h>

namespace libshape
{

class PointShape : public d2d::IShape
{
public:
	PointShape();
	PointShape(const d2d::Vector& pos);
	PointShape(const PointShape& point);

	//
	// IObject interface
	//
	virtual PointShape* clone() const;

	//
	// IShape interface
	//
	virtual const char* GetShapeDesc() const { return "point"; }
	virtual bool isContain(const d2d::Vector& pos) const;
	virtual bool isIntersect(const d2d::Rect& rect) const;
	virtual void Translate(const d2d::Vector& offset);
	virtual const d2d::Rect& getRect() const { return m_rect; }
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;
	virtual d2d::IPropertySetting* createPropertySetting(d2d::EditPanel* editPanel);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	const d2d::Vector& GetPos() const { return m_pos; }
	void SetPos(const d2d::Vector& pos) { m_pos = pos; }

private:
	static const int RADIUS = 5;

private:
	d2d::Vector m_pos;

	d2d::Rect m_rect;

}; // PointShape

}

#endif // _LIBSHAPE_POINT_SHAPE_H_
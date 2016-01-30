#ifndef _LIBSHAPE_POINT_SHAPE_H_
#define _LIBSHAPE_POINT_SHAPE_H_

#include <drag2d.h>

namespace libshape
{

class PointShape : public d2d::Shape
{
public:
	PointShape();
	PointShape(const d2d::Vector& pos);
	PointShape(const PointShape& point);

	//
	// IObject interface
	//
	virtual PointShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "point"; }
	virtual bool IsContain(const d2d::Vector& pos) const;
	virtual bool IsIntersect(const d2d::Rect& rect) const;
	virtual void Translate(const d2d::Vector& offset);
	virtual const d2d::Rect& GetRect() const { return m_rect; }
	virtual void Draw(const d2d::Matrix& mt,
		const d2d::ColorTrans& color = d2d::ColorTrans()) const;
	virtual d2d::IPropertySetting* CreatePropertySetting(d2d::EditPanelImpl* stage);
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
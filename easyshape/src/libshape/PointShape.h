#ifndef _EASYSHAPE_POINT_SHAPE_H_
#define _EASYSHAPE_POINT_SHAPE_H_

#include <ee/Shape.h>
#include <ee/Vector.h>
#include <ee/Rect.h>

namespace eshape
{

class PointShape : public ee::Shape
{
public:
	PointShape();
	PointShape(const ee::Vector& pos);
	PointShape(const PointShape& point);

	//
	// IObject interface
	//
	virtual PointShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "point"; }
	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;
	virtual void Translate(const ee::Vector& offset);
	virtual const ee::Rect& GetRect() const { return m_rect; }
	virtual void Draw(const sm::mat4& mt,
		const ee::RenderColor& color = ee::RenderColor()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	const ee::Vector& GetPos() const { return m_pos; }
	void SetPos(const ee::Vector& pos) { m_pos = pos; }

private:
	ee::Vector m_pos;

	ee::Rect m_rect;

}; // PointShape

}

#endif // _EASYSHAPE_POINT_SHAPE_H_
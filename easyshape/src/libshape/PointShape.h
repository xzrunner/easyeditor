#ifndef _EASYSHAPE_POINT_SHAPE_H_
#define _EASYSHAPE_POINT_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/PointShape.h>

namespace eshape
{

class PointShape : public ee::Shape, private s2::PointShape
{
public:
	PointShape() {}
	PointShape(const sm::vec2& pos) 
		: s2::PointShape(pos) 
	{}

	//
	// IObject interface
	//
	virtual PointShape* Clone() const {
		return new PointShape(*this);
	}

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "point"; }
	virtual bool IsContain(const sm::vec2& pos) const {
		return s2::PointShape::IsContain(pos);
	}
	virtual bool IsIntersect(const sm::rect& rect) const {
		return s2::PointShape::IsIntersect(rect);
	}
	virtual void Translate(const sm::vec2& offset) {
		SetPos(m_pos + offset);
	}
	virtual const sm::rect& GetRect() const { return s2::PointShape::GetBounding(); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color = s2::RenderColor()) const {
		s2::PointShape::Draw(mt, color);
	}
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) {
		return NULL;
	}
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	const sm::vec2& GetPos() const { 
		return m_pos; 
	}
	void SetPos(const sm::vec2& pos) { 
		m_pos = pos;
		UpdateBounding();
	}

}; // PointShape

}

#endif // _EASYSHAPE_POINT_SHAPE_H_
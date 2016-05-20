#ifndef _EASYSHAPE_POINT_SHAPE_H_
#define _EASYSHAPE_POINT_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/PointShape.h>

namespace eshape
{

class PointShape : public ee::Shape
{
public:
	PointShape() {}
	PointShape(const sm::vec2& pos) 
		: m_core(pos) 
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
		return m_core.IsContain(pos);
	}
	virtual bool IsIntersect(const sm::rect& rect) const {
		return m_core.IsIntersect(rect);
	}
	virtual void Translate(const sm::vec2& offset) {
		m_core.SetPos(m_core.GetPos() + offset);
	}
	virtual const sm::rect& GetRect() const { return m_core.GetRegion(); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color = s2::RenderColor()) const {
		m_core.Draw(mt, color);
	}
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) {
		return NULL;
	}
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	const sm::vec2& GetPos() const { 
		return m_core.GetPos(); 
	}
	void SetPos(const sm::vec2& pos) { 
		m_core.SetPos(pos); 
	}

private:
	s2::PointShape m_core;

}; // PointShape

}

#endif // _EASYSHAPE_POINT_SHAPE_H_
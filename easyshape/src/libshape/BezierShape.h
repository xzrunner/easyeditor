#ifndef _EASYSHAPE_BEZIER_SHAPE_H_
#define _EASYSHAPE_BEZIER_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/BezierShape.h>

namespace eshape
{

class BezierShape : public ee::Shape, private s2::BezierShape
{
public:
	BezierShape() {}
	BezierShape(const sm::vec2& start, const sm::vec2& end);

	//
	// IObject interface
	//
	virtual BezierShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "bezier"; }
	virtual bool IsContain(const sm::vec2& pos) const { return s2::BezierShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::BezierShape::IsIntersect(rect); }
	virtual void Translate(const sm::vec2& offset);
	virtual const sm::rect& GetRect() const { return s2::BezierShape::GetBounding(); }
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const { s2::BezierShape::Draw(mt, color); }
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void Mirror(bool x, bool y);

	void MoveCtrlNode(const sm::vec2& from, const sm::vec2& to);

	const sm::vec2* GetCtrlNodes() const { return m_control_nodes; }

}; // BezierShape

}

#endif // _EASYSHAPE_BEZIER_SHAPE_H_
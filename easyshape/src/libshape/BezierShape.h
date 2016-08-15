#ifndef _EASYSHAPE_BEZIER_SHAPE_H_
#define _EASYSHAPE_BEZIER_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/BezierShape.h>

namespace eshape
{

class BezierShape : public ee::Shape, public s2::BezierShape
{
public:
	BezierShape() {}
	BezierShape(const BezierShape& bezier) {}
	BezierShape(const sm::vec2& start, const sm::vec2& end);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual bool IsContain(const sm::vec2& pos) const { return s2::BezierShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::BezierShape::IsIntersect(rect); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color = s2::RenderColor()) const { s2::BezierShape::Draw(mt, color); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual BezierShape* EEClone() const { return new BezierShape(*this); }
	virtual const char* GetShapeDesc() const { return "bezier"; }
	virtual void Translate(const sm::vec2& offset);
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void Mirror(bool x, bool y);

	void MoveCtrlNode(const sm::vec2& from, const sm::vec2& to);

	const sm::vec2* GetCtrlNodes() const { return m_control_nodes; }

protected:
	virtual void UpdateBounding() { s2::BezierShape::UpdateBounding(); }

}; // BezierShape

}

#endif // _EASYSHAPE_BEZIER_SHAPE_H_
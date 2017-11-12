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
	BezierShape(const BezierShape& bezier);
	BezierShape(const sm::vec2& start, const sm::vec2& end);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual BezierShape* Clone() const override { return new BezierShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const override { return s2::BezierShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const override { return s2::BezierShape::IsIntersect(rect); }
	virtual void Draw(const s2::RenderParams& rp) const override { s2::BezierShape::Draw(rp); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const override { return "bezier"; }
	virtual void Translate(const sm::vec2& offset) override;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override;
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) override;
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const override;

	void Mirror(bool x, bool y);

	void MoveCtrlNode(const sm::vec2& from, const sm::vec2& to);

	const sm::vec2* GetCtrlNodes() const { return m_control_nodes; }

protected:
	virtual void UpdateBounding() override { s2::BezierShape::UpdateBounding(); }

}; // BezierShape

}

#endif // _EASYSHAPE_BEZIER_SHAPE_H_
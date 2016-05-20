#ifndef _EASYSHAPE_BEZIER_SHAPE_H_
#define _EASYSHAPE_BEZIER_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/BezierShape.h>

namespace eshape
{

class BezierShape : public ee::Shape
{
public:
	BezierShape() {}
//	BezierShape(const sm::vec2 points[4]);
	BezierShape(const sm::vec2& start, const sm::vec2& end);

	//
	// IObject interface
	//
	virtual BezierShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "bezier"; }
	virtual bool IsContain(const sm::vec2& pos) const { return m_core.IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return m_core.IsIntersect(rect); }
	virtual void Translate(const sm::vec2& offset);
	virtual const sm::rect& GetRect() const { return m_core.GetRegion(); }
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const { m_core.Draw(mt, color); }
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void Mirror(bool x, bool y);

	void MoveCtrlNode(const sm::vec2& from, const sm::vec2& to);

	const sm::vec2* GetCtrlNodes() const { return m_core.GetCtrlNodes(); }

private:
	s2::BezierShape m_core;

}; // BezierShape

}

#endif // _EASYSHAPE_BEZIER_SHAPE_H_
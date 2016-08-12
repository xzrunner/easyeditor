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
	BezierShape(const sm::vec2& start, const sm::vec2& end);

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

}; // BezierShape

}

#endif // _EASYSHAPE_BEZIER_SHAPE_H_
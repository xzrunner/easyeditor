#ifndef _EASYSHAPE_BEZIER_SHAPE_H_
#define _EASYSHAPE_BEZIER_SHAPE_H_

#include "ChainShape.h"

namespace eshape
{

class BezierShape : public ChainShape
{
public:
	BezierShape();
	BezierShape(const BezierShape& bezier);
	BezierShape(const sm::vec2 points[4]);
	BezierShape(const sm::vec2& start, const sm::vec2& end);
	virtual ~BezierShape();

	//
	// IObject interface
	//
	virtual BezierShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "bezier"; }
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual void Translate(const sm::vec2& offset);
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void createCurve();

	void Mirror(bool x, bool y);

	void MoveCtrlNode(const sm::vec2& from, const sm::vec2& to);

	const sm::vec2* GetCtrlNodes() const { return m_control_nodes; }

public:
	static const int CTRL_NODE_COUNT = 4;

private:
	void CopyCtrlNodes(const sm::vec2 ctrl_points[]);

	sm::vec2 pointOnCubicBezier(float t);

private:
	static const int RADIUS = 5;

private:
	// [0] start, [1] ctrl1, [2] ctrl2, [3] end
	sm::vec2 m_control_nodes[CTRL_NODE_COUNT];

}; // BezierShape

}

#endif // _EASYSHAPE_BEZIER_SHAPE_H_
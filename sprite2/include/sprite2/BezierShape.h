#ifndef _SPRITE2_BEZIER_SHAPE_H_
#define _SPRITE2_BEZIER_SHAPE_H_

#include "PolylineShape.h"

namespace s2
{

class BezierShape : public PolylineShape
{
public:
	BezierShape() {}
	BezierShape(const sm::vec2& start, const sm::vec2& end);
	
	virtual bool IsContain(const sm::vec2& pos) const;

	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const;

	const sm::vec2* GetCtrlNodes() const { return m_control_nodes; }
	void SetCtrlNodes(const sm::vec2* ctrl_nodes);

private:
// 	void CopyCtrlNodes(const sm::vec2* ctrl_points) {
// 		memcpy(m_control_nodes, ctrl_points, sizeof(m_control_nodes));
// 	}

	void BuildCurve();
	sm::vec2 PointOnCubicBezier(float t);

public:
	static const int CTRL_NODE_COUNT = 4;

private:
	// [0] start, [1] ctrl1, [2] ctrl2, [3] end
	sm::vec2 m_control_nodes[CTRL_NODE_COUNT];

}; // BezierShape

}

#endif // _SPRITE2_BEZIER_SHAPE_H_
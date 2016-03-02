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
	BezierShape(const ee::Vector points[4]);
	BezierShape(const ee::Vector& start, const ee::Vector& end);
	virtual ~BezierShape();

	//
	// IObject interface
	//
	virtual BezierShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "bezier"; }
	virtual bool IsContain(const ee::Vector& pos) const;
	virtual void Translate(const ee::Vector& offset);
	virtual void Draw(const ee::Matrix& mt,
		const ee::ColorTrans& color = ee::ColorTrans()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void createCurve();

	void Mirror(bool x, bool y);

	void MoveCtrlNode(const ee::Vector& from, const ee::Vector& to);

	const ee::Vector* GetCtrlNode() const { return m_control_nodes; }

public:
	static const int CTRL_NODE_COUNT = 4;

private:
	void CopyCtrlNodes(const ee::Vector ctrl_points[]);

	ee::Vector pointOnCubicBezier(float t);

private:
	static const int RADIUS = 5;

private:
	// [0] start, [1] ctrl1, [2] ctrl2, [3] end
	ee::Vector m_control_nodes[CTRL_NODE_COUNT];

}; // BezierShape

}

#endif // _EASYSHAPE_BEZIER_SHAPE_H_
#ifndef _LIBSHAPE_BEZIER_SHAPE_H_
#define _LIBSHAPE_BEZIER_SHAPE_H_

#include "ChainShape.h"

#include <drag2d.h>

namespace libshape
{

class BezierShape : public ChainShape
{
public:
	BezierShape(const BezierShape& bezier);
	BezierShape(const d2d::Vector points[4]);
	BezierShape(const d2d::Vector& start, const d2d::Vector& end);
	virtual ~BezierShape();

	//
	// IObject interface
	//
	virtual BezierShape* clone() const;

	//
	// IShape interface
	//
	virtual bool isContain(const d2d::Vector& pos) const;
	virtual void Translate(const d2d::Vector& offset);
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;
	virtual d2d::IPropertySetting* createPropertySetting(d2d::EditPanel* editPanel);

	void createCurve();

	void Mirror(bool x, bool y);

	void MoveCtrlNode(const d2d::Vector& from, const d2d::Vector& to);

	const d2d::Vector* GetCtrlNode() const { return m_control_nodes; }

public:
	static const int CTRL_NODE_COUNT = 4;

private:
	void CopyCtrlNodes(const d2d::Vector ctrl_points[]);

	d2d::Vector pointOnCubicBezier(float t);

private:
	static const int RADIUS = 5;

private:
	// [0] start, [1] ctrl1, [2] ctrl2, [3] end
	d2d::Vector m_control_nodes[CTRL_NODE_COUNT];

private:
	d2d::ShapeStyle m_style;

}; // BezierShape

}

#endif // _LIBSHAPE_BEZIER_SHAPE_H_
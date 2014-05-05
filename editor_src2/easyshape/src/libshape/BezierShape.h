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
	virtual void draw(const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;
	virtual d2d::IPropertySetting* createPropertySetting(d2d::EditPanel* editPanel);

	void createCurve();

private:
	d2d::Vector pointOnCubicBezier(float t);

private:
	static const int RADIUS = 5;

public:
	// [0] start, [1] ctrl1, [2] ctrl2, [3] end
	d2d::Vector points[4];

private:
	d2d::ShapeStyle m_style;

}; // BezierShape

}

#endif // _LIBSHAPE_BEZIER_SHAPE_H_
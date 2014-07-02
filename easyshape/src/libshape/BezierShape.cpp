#include "BezierShape.h"
#include "BezierPropertySetting.h"

namespace libshape
{

BezierShape::BezierShape(const BezierShape& bezier)
{
	for (size_t i = 0; i < 4; ++i)
		points[i] = bezier.points[i];
	createCurve();
}

BezierShape::BezierShape(const d2d::Vector points[4])
{
	for (size_t i = 0; i < 4; ++i)
		this->points[i] = points[i];
	createCurve();
}

BezierShape::BezierShape(const d2d::Vector& start, const d2d::Vector& end)
{
	points[0] = start;
	points[3] = end;

	d2d::Vector mid = (start + end) * 0.5f;
	d2d::Vector offset = (end - start) * 0.5f;
	points[1] = mid + d2d::Math::rotateVectorRightAngle(offset, true);
	points[2] = mid + d2d::Math::rotateVectorRightAngle(offset, false);

	createCurve();
}

BezierShape::~BezierShape()
{

}

BezierShape* BezierShape::clone() const
{
	return new BezierShape(*this);
}

bool BezierShape::isContain(const d2d::Vector& pos) const
{
	bool ret = false;
	for (size_t i = 0; i < 4; ++i)
	{
		if (d2d::Math::getDistance(pos, points[i]) < RADIUS)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void BezierShape::draw(const d2d::Screen& scr, const d2d::Colorf& color/* = Colorf(0, 0, 0)*/) const
{
	ChainShape::draw(scr, color);

	for (size_t i = 0; i < 4; ++i) {
		d2d::PrimitiveDraw::rect(scr, points[i], (float)RADIUS, (float)RADIUS, m_style);
	}
}

d2d::IPropertySetting* BezierShape::createPropertySetting(d2d::EditPanel* editPanel)
{
	return new BezierPropertySetting(editPanel, this);
}

void BezierShape::createCurve()
{
	const size_t num = std::max(20, (int)(d2d::Math::getDistance(points[0], points[3]) / 10));
	float dt = 1.0f / (num - 1);
	std::vector<d2d::Vector> vertices(num);
	for (size_t i = 0; i < num; ++i)
		vertices[i] = pointOnCubicBezier(i * dt);
	setVertices(vertices);
}

d2d::Vector BezierShape::pointOnCubicBezier(float t)
{
	float ax, bx, cx;
	float ay, by, cy;
	float tSquared, tCubed;
	d2d::Vector result;

	cx = 3.0f * (points[1].x - points[0].x);
	bx = 3.0f * (points[2].x - points[1].x) - cx;
	ax = points[3].x - points[0].x - cx - bx;

	cy = 3.0f * (points[1].y - points[0].y);
	by = 3.0f * (points[2].y - points[1].y) - cy;
	ay = points[3].y - points[0].y - cy - by;

	tSquared = t * t;
	tCubed = tSquared * t;

	result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + points[0].x;
	result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + points[0].y;

	return result;
}

}
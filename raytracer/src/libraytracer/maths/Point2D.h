#ifndef _RT_POINT2D_H_
#define _RT_POINT2D_H_

namespace rt
{

class Point2D
{
public:
	float x, y;

public:
	Point2D();
	Point2D(float x, float y);

	Point2D operator * (const float a) const;

}; // Point2D

inline Point2D::Point2D()
	: x(0), y(0) {}

inline Point2D::Point2D(float x, float y)
	: x(x), y(y) {}

inline Point2D Point2D::operator * (const float a) const {
	return Point2D(x * a, y * a);
}

}

#endif // _RT_POINT2D_H_
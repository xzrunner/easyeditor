#ifndef _EASYEDITOR_RVG_H_
#define _EASYEDITOR_RVG_H_

#include <vector>

namespace ee
{

class Vector;
class Colorf;
struct ShapeStyle;
class Rect;

class RVG
{
public:
	// config
	static void Init();	

	// point
	static void Point(const Vector& vertex, const Colorf& color, float size = 2);
	static void Points(const std::vector<Vector>& vertices, const Colorf& color, float size = 2);

	// line
	static void Line(const Vector& p0, const Vector& p1, const Colorf& color, float width = 2);
	static void DotLine(const Vector& p0, const Vector& p1, const Colorf& color, float width = 2);
	static void DashLine(const Vector& p0, const Vector& p1, const Colorf& color, float width = 2);
	static void DotDashLine(const Vector& p0, const Vector& p1, const Colorf& color, float width = 2);

	static void Lines(const std::vector<Vector>& vertices, const Colorf& color, float width = 2);
	static void Polyline(const std::vector<Vector>& vertices, const Colorf& color, bool loop, float width = 2);

	// face
	static void Triangles(const std::vector<Vector>& triangles, const Colorf& color);

	static void Rect(const Vector& center, float radius, const ShapeStyle& style);
	static void Rect(const Vector& center, float hw, float hh, const ShapeStyle& style);
	static void Rect(const Vector& p0, const Vector& p1, const ShapeStyle& style);

	static void Circle(const Vector& center, float radius, bool filling, const Colorf& color, int segments = 16);
	static void Circles(const std::vector<Vector>& circles, float radius, bool filling, const Colorf& color, int segments = 16);

	// compose
	static void Cross(const Vector& center, float edge, const Colorf& color, float width = 2);
	static void Cross(const Vector& center, float xedge, float yedge, const Colorf& color, float width = 2);

}; // RVG

}

#endif // _EASYEDITOR_RVG_H_
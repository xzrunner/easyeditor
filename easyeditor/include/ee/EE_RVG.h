#ifndef _EASYEDITOR_RVG_H_
#define _EASYEDITOR_RVG_H_

#include <vector>

namespace ee
{

class Vector;
class Colorf;
class Rect;

class RVG
{
public:
	// config
	static void Init();	

	static void Color(const Colorf& color);
	static void PointSize(float size);
	static void LineWidth(float width);

	// point
	static void Point(const Vector& vertex);
	static void Points(const std::vector<Vector>& vertices);

	// line
	static void Line(const Vector& p0, const Vector& p1);
	static void DotLine(const Vector& p0, const Vector& p1);
	static void DashLine(const Vector& p0, const Vector& p1);
	static void DotDashLine(const Vector& p0, const Vector& p1);

	static void Lines(const std::vector<Vector>& vertices);
	static void Polyline(const std::vector<Vector>& vertices, bool loop);

	// face
	static void Triangles(const std::vector<Vector>& triangles);
	static void Triangles(const std::vector<Vector>& triangles, const std::vector<Colorf>& colors);
	static void TriangleStrip(const std::vector<Vector>& triangles);

	static void Rect(const Vector& center, float hw, float hh, bool filling);
	static void Rect(const Vector& p0, const Vector& p1, bool filling);

	static void Circle(const Vector& center, float radius, bool filling, int segments = 16);
	static void Circles(const std::vector<Vector>& circles, float radius, bool filling, int segments = 16);

	// compose
	static void Cross(const Vector& center, float edge);
	static void Cross(const Vector& center, float xedge, float yedge);

}; // RVG

}

#endif // _EASYEDITOR_RVG_H_
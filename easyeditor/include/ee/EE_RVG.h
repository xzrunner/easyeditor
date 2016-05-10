#ifndef _EASYEDITOR_RVG_H_
#define _EASYEDITOR_RVG_H_

#include <SM_Vector.h>
#include <sprite2/Color.h>

#include <vector>

namespace ee
{

class Rect;

class RVG
{
public:
	// config
	static void Init();	

	static void Color(const s2::Color& color);
	static void PointSize(float size);
	static void LineWidth(float width);

	// point
	static void Point(const sm::vec2& vertex);
	static void Points(const std::vector<sm::vec2>& vertices);

	// line
	static void Line(const sm::vec2& p0, const sm::vec2& p1);
	static void DotLine(const sm::vec2& p0, const sm::vec2& p1);
	static void DashLine(const sm::vec2& p0, const sm::vec2& p1);
	static void DotDashLine(const sm::vec2& p0, const sm::vec2& p1);

	static void Lines(const std::vector<sm::vec2>& vertices);
	static void Polyline(const std::vector<sm::vec2>& vertices, bool loop);

	// face
	static void Triangles(const std::vector<sm::vec2>& triangles);
	static void Triangles(const std::vector<sm::vec2>& triangles, const std::vector<s2::Color>& colors);
	static void TriangleStrip(const std::vector<sm::vec2>& triangles);

	static void Rect(const sm::vec2& center, float hw, float hh, bool filling);
	static void Rect(const sm::vec2& p0, const sm::vec2& p1, bool filling);

	static void Circle(const sm::vec2& center, float radius, bool filling, int segments = 16);
	static void Circles(const std::vector<sm::vec2>& circles, float radius, bool filling, int segments = 16);

	// compose
	static void Cross(const sm::vec2& center, float edge);
	static void Cross(const sm::vec2& center, float xedge, float yedge);

}; // RVG

}

#endif // _EASYEDITOR_RVG_H_
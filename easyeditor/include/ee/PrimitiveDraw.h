#ifndef _EASYEDITOR_PRIMITIVE_DRAW_H_
#define _EASYEDITOR_PRIMITIVE_DRAW_H_

#include "ShapeStyle.h"
#include "Color.h"

#include <vector>

namespace ee
{

class Vector;
class Rect;
class Matrix;

class PrimitiveDraw
{
public:
	static void DrawRect(const Vector& center, float radius, const ShapeStyle& style);
	static void DrawRect(const Vector& center, float hWidth, float hHeight, const ShapeStyle& style);
	static void DrawRect(const Rect& rect, const ShapeStyle& style);
	static void DrawRect(const Vector& p0, const Vector& p1, const ShapeStyle& style);

	static void DrawCircle(const Vector& center, float radius, bool isFill = false, 
		float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);
	static void DrawCircles(const std::vector<Vector>& circles, float radius, bool isFill = false, 
		float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);

	static void DrawCircle(const Matrix& mt, const Vector& center, float radius, bool isFill = false, 
		float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);

	static void DrawPoint(const Vector& vertex, const Colorf& color, float size = 2);
	static void DrawPoints(const std::vector<Vector>& vertices, const Colorf& color, float size = 2);
	static void DrawLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
	static void DrawDotLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
	static void DrawDashLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
	static void DrawDotDashLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
	static void DrawLines(const std::vector<Vector>& vertices, const Colorf& color, float size = 2);
	static void DrawPolyline(const std::vector<Vector>& vertices, const Colorf& color, bool isClose, float size = 2);
	static void DrawPolygon(const std::vector<Vector>& vertices, const Colorf& color);
	static void DrawTriangles(const std::vector<Vector>& triangles, const Colorf& color);
	static void DrawTrianglesLine(const std::vector<Vector>& triangles, const Colorf& color, float size = 2);

	static void Cross(const Vector& center, float edge, const Colorf& color, float size = 2);
	static void Cross(const Vector& center, float xedge, float yedge, const Colorf& color, float size = 2);

	// with matrix

	static void DrawRect(const Matrix& mt, float hWidth, float hHeight, const ShapeStyle& style);
	static void DrawRect(const Matrix& mt, const Rect& r, const ShapeStyle& style);
	static void DrawRect(const Matrix& mt, const Vector& p0, const Vector& p1, const ShapeStyle& style);

	static void DrawPoint(const Matrix& mt, const Vector& vertex, const Colorf& color, float size = 2);

	static void DrawPolyline(const Matrix& mt, const std::vector<Vector>& vertices, const Colorf& color, bool isClose, float size = 2);
	static void DrawTriangles(const Matrix& mt, const std::vector<Vector>& triangles, const Colorf& color);
	static void DrawTriangles(const Matrix& mt, const std::vector<Vector>& triangles, const std::vector<Colorf>& colors);

	static void MatrixTrans(const Matrix& mt, const std::vector<Vector>& src, std::vector<Vector>& dst);

}; // PrimitiveDraw

}

#endif // _EASYEDITOR_PRIMITIVE_DRAW_H_

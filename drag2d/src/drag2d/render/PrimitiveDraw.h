#pragma once

#include <vector>

#include "ShapeStyle.h"
#include "common/Color.h"

namespace d2d
{
	class Vector;
	class Rect;
	struct Colorf;
	class Screen;
	class Matrix;

	class PrimitiveDraw
	{
	public:
		static void resetColorAndTexture();

		static void rect(const Screen& scr, const Vector& center, float radius, const ShapeStyle& style);
		static void rect(const Screen& scr, const Vector& center, float hWidth, float hHeight, const ShapeStyle& style);
		static void rect(const Screen& scr, const Rect& rect, const ShapeStyle& style);
		static void rect(const Screen& scr, const Vector& p0, const Vector& p1, const ShapeStyle& style);

		static void rect(const Screen& scr, const Matrix& mt, float hWidth, float hHeight, const ShapeStyle& style);
		static void rect(const Screen& scr, const Matrix& mt, const Vector& p0, const Vector& p1, const ShapeStyle& style);

		static void drawCircle(const Screen& scr, const Vector& center, float radius, bool isFill = false, 
			float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);
		static void drawCircles(const Screen& scr, const std::vector<Vector>& circles, float radius, bool isFill = false, 
			float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);

		static void drawPoints(const Screen& scr, const std::vector<Vector>& vertices, const Colorf& color, float size = 2);
		static void drawLine(const Screen& scr, const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDotLine(const Screen& scr, const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDashLine(const Screen& scr, const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDotDashLine(const Screen& scr, const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawLines(const Screen& scr, const std::vector<Vector>& vertices, const Colorf& color, float size = 2);
		static void drawPolyline(const Screen& scr, const std::vector<Vector>& vertices, const Colorf& color, bool isClose, float size = 2);
		static void drawPolygon(const Screen& scr, const std::vector<Vector>& vertices, const Colorf& color);
		static void drawTriangles(const Screen& scr, const std::vector<Vector>& triangles, const Colorf& color);
		// 		static void drawTriangles(const std::vector<Vector>& triangles, const wxString& filepath);
		// 		static void drawTriangles(const std::vector<Vector>& triangles, const wxString& filepath, 
		// 			const Vector& leftLow, const std::vector<unsigned int>& mesh);
		static void drawTriangles(const Screen& scr, unsigned int texID, const std::vector<Vector>& triangles, 
			const std::vector<Vector>& texCoords);
		static void drawTrianglesSlow(const Screen& scr, unsigned int texID, const std::vector<Vector>& triangles, 
			const std::vector<Vector>& texCoords);

		static void cross(const Screen& scr, const Vector& center, float edge, const Colorf& color, float size = 2);
		static void cross(const Screen& scr, const Vector& center, float xedge, float yedge, const Colorf& color, float size = 2);

		static void text(const char* text);

	private:
		static void lineStypeBegin(const LineStyle& style);
		static void lineStypeEnd(const LineStyle& style);

	}; // PrimitiveDraw
}


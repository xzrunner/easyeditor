#pragma once

#include <vector>

#include "ShapeStyle.h"
#include "common/Color.h"

namespace d2d
{
	class Vector;
	class Rect;
	struct Colorf;

	class PrimitiveDraw
	{
	public:
		static void resetColorAndTexture();

		static void rect(const Vector& center, float radius, const ShapeStyle& style);
		static void rect(const Vector& center, float hWidth, float hHeight, const ShapeStyle& style);
		static void rect(const Rect& rect, const ShapeStyle& style);
		static void rect(const Vector& p0, const Vector& p1, const ShapeStyle& style);

		static void drawCircle(const Vector& center, float radius, bool isFill = false, 
			float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);
		static void drawCircles(const std::vector<Vector>& circles, float radius, bool isFill = false, 
			float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);

		static void drawPoints(const std::vector<Vector>& vertices, const Colorf& color, float size = 2);
		static void drawLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDotLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDashLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDotDashLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawLines(const std::vector<Vector>& vertices, const Colorf& color, float size = 2);
		static void drawPolyline(const std::vector<Vector>& vertices, const Colorf& color, bool isClose, float size = 2);
		static void drawPolygon(const std::vector<Vector>& vertices, const Colorf& color);
		static void drawTriangles(const std::vector<Vector>& triangles, const Colorf& color);
		// 		static void drawTriangles(const std::vector<Vector>& triangles, const wxString& filepath);
		// 		static void drawTriangles(const std::vector<Vector>& triangles, const wxString& filepath, 
		// 			const Vector& leftLow, const std::vector<unsigned int>& mesh);
		static void drawTriangles(unsigned int texID, const std::vector<Vector>& triangles, 
			const std::vector<Vector>& texCoords);
		static void drawTrianglesSlow(unsigned int texID, const std::vector<Vector>& triangles, 
			const std::vector<Vector>& texCoords);

		static void cross(const Vector& center, float edge, const Colorf& color, float size = 2);
		static void cross(const Vector& center, float xedge, float yedge, const Colorf& color, float size = 2);

	private:
		static void lineStypeBegin(const LineStype& style);
		static void lineStypeEnd(const LineStype& style);

	}; // PrimitiveDraw
}


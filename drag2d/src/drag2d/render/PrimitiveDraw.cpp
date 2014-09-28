#include "PrimitiveDraw.h"
#include "PrimitiveDrawNew.h"
#include "GL10.h"

#include "dataset/ISymbol.h"
#include "dataset/SymbolMgr.h"
#include "dataset/Font.h"
#include "dataset/FontSymbol.h"
#include "common/Vector.h"
#include "common/Rect.h"
#include "common/Color.h"
#include "common/Math.h"
#include "render/ShaderNew.h"

namespace d2d
{

static const int MAX_VERTICES = 4096;
static float VERTICES[MAX_VERTICES * 2];

void PrimitiveDraw::rect(const Vector& center, float radius, const ShapeStyle& style)
{
	rect(center, radius, radius, style);
}

void PrimitiveDraw::rect(const Vector& center, float hWidth, float hHeight, const ShapeStyle& style)
{
	rect(center - Vector(hWidth, hHeight), center + Vector(hWidth, hHeight), style);
}

void PrimitiveDraw::rect(const Rect& r, const ShapeStyle& style)
{
	rect(Vector(r.xMin, r.yMin), Vector(r.xMax, r.yMax), style);
}

void PrimitiveDraw::rect(const Vector& p0, const Vector& p1, const ShapeStyle& style)
{
	int type = style.fill ? GL10::GL_QUADS : GL10::GL_LINE_LOOP;
	if (!style.fill) {
		PrimitiveDrawNew::SetLineWidth(style.size);
	}

	int idx = 0;
	VERTICES[idx++] = p0.x;
	VERTICES[idx++] = p0.y;

	VERTICES[idx++] = p0.x;
	VERTICES[idx++] = p1.y;

	VERTICES[idx++] = p1.x;
	VERTICES[idx++] = p1.y;

	VERTICES[idx++] = p1.x;
	VERTICES[idx++] = p0.y;

	PrimitiveDrawNew::SetColor(style.color);
	PrimitiveDrawNew::LineStypeBegin(style.lineStyle);
	PrimitiveDrawNew::Draw(type, VERTICES, 4);
	PrimitiveDrawNew::LineStypeEnd(style.lineStyle);
}

void PrimitiveDraw::drawCircle(const Vector& center, float radius, bool isFill/* = false*/, 
							   float size/* = 2*/, const Colorf& color/* = Colorf(0, 0, 0)*/, size_t kSegments/* = 16*/)
{
	PrimitiveDrawNew::SetColor(color);
	const float k_increment = 2.0f * PI / kSegments;
	float theta = 0.0f;
	if (!isFill)
	{
		PrimitiveDrawNew::SetLineWidth(size);
		int idx = 0;
 		for (size_t i = 0; i < kSegments; ++i)
 		{
 			Vector v = center + Vector(cosf(theta), sinf(theta)) * radius;
			VERTICES[idx++] = v.x;
			VERTICES[idx++] = v.y;
 			theta += k_increment;
 		}
		PrimitiveDrawNew::Draw(GL10::GL_LINE_LOOP, VERTICES, kSegments);
	}
	else
	{
		int idx = 0;
		VERTICES[idx++] = center.x;
		VERTICES[idx++] = center.y;
		for (size_t i = 0; i < kSegments; ++i)
		{
			Vector v = center + Vector(cosf(theta), sinf(theta)) * radius;
			VERTICES[idx++] = v.x;
			VERTICES[idx++] = v.y;
			theta += k_increment;
		}
		VERTICES[idx++] = VERTICES[2];
		VERTICES[idx++] = VERTICES[3];
		PrimitiveDrawNew::Draw(GL10::GL_TRIANGLE_FAN, VERTICES, kSegments + 2);
	}
}

void PrimitiveDraw::drawCircles(const std::vector<Vector>& circles, float radius, bool isFill/* = false*/, 
								float size/* = 2*/, const Colorf& color/* = Colorf(0, 0, 0)*/, size_t kSegments/* = 16*/)
{
	if (!isFill)
	{
		for (size_t i = 0, n = circles.size(); i < n; ++i) {
			drawCircle(circles[i], radius, isFill, size, color, kSegments);
		}
	}
	else
	{
		if (circles.empty()) return;

		PrimitiveDrawNew::SetColor(color);
		
		int idx = 0;
		const float k_increment = 2.0f * PI / kSegments;
		for (size_t i = 0, n = circles.size(); i < n; ++i)
		{
			float theta = 0.0f;
			Vector lastPos;
			for (size_t j = 0; j < kSegments; ++j)
			{
				if (j == 0) {
					VERTICES[idx++] = circles[i].x + cosf(theta) * radius;
					VERTICES[idx++] = circles[i].y + sinf(theta) * radius;
				} else {
					VERTICES[idx++] = lastPos.x;
					VERTICES[idx++] = lastPos.y;
				}
				lastPos = circles[i] + Vector(cosf(theta + k_increment), sinf(theta + k_increment)) * radius;
				VERTICES[idx++] = lastPos.x;
				VERTICES[idx++] = lastPos.y;
				VERTICES[idx++] = circles[i].x;
				VERTICES[idx++] = circles[i].y;

				theta += k_increment;
			}
		}
		assert(idx < MAX_VERTICES * 2);
		PrimitiveDrawNew::Draw(GL10::GL_TRIANGLES, VERTICES, idx >> 1);
	}
}

void PrimitiveDraw::drawPoints(const std::vector<Vector>& vertices, const Colorf& color, float size/* = 2*/)
{
	if (vertices.empty()) {
		return;
	}

	PrimitiveDrawNew::SetPointSize(size);
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(GL10::GL_POINTS, &vertices[0].x, vertices.size());
}

void PrimitiveDraw::drawLine(const Vector& p0, const Vector& p1, 
							 const Colorf& color, float size/* = 2*/)
{
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::SetLineWidth(size);

	VERTICES[0] = p0.x;
	VERTICES[1] = p0.y;
	VERTICES[2] = p1.x;
	VERTICES[3] = p1.y;
	PrimitiveDrawNew::Draw(GL10::GL_LINE_STRIP, VERTICES, 2);
}

void PrimitiveDraw::drawDotLine(const Vector& p0, const Vector& p1, 
								const Colorf& color, float size /*= 2*/)
{
	PrimitiveDrawNew::LineStypeBegin(LS_DOT);
	drawLine(p0, p1, color, size);
	PrimitiveDrawNew::LineStypeEnd(LS_DOT);
}

void PrimitiveDraw::drawDashLine(const Vector& p0, const Vector& p1, 
								 const Colorf& color, float size /*= 2*/)
{
	PrimitiveDrawNew::LineStypeBegin(LS_DASH);
	drawLine(p0, p1, color, size);
	PrimitiveDrawNew::LineStypeEnd(LS_DASH);
}

void PrimitiveDraw::drawDotDashLine(const Vector& p0, const Vector& p1, 
									const Colorf& color, float size /*= 2*/)
{
	PrimitiveDrawNew::LineStypeBegin(LS_DOT_DASH);
	drawLine(p0, p1, color, size);
	PrimitiveDrawNew::LineStypeEnd(LS_DOT_DASH);
}

void PrimitiveDraw::drawLines(const std::vector<Vector>& vertices, 
							  const Colorf& color, float size /*= 2*/)
{
	PrimitiveDrawNew::SetLineWidth(size);
	PrimitiveDrawNew::Draw(GL10::GL_LINES, &vertices[0].x, vertices.size());
}

void PrimitiveDraw::drawPolyline(const std::vector<Vector>& vertices, 
								 const Colorf& color, bool isClose, float size /*= 2*/)
{
	int type = isClose ? GL10::GL_LINE_LOOP : GL10::GL_LINE_STRIP;
	PrimitiveDrawNew::SetLineWidth(size);
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(type, &vertices[0].x, vertices.size());
}
 
void PrimitiveDraw::drawPolygon(const std::vector<Vector>& vertices, const Colorf& color)
{
	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(GL10::GL_POLYGON, &vertices[0].x, vertices.size());
}

void PrimitiveDraw::drawTriangles(const std::vector<Vector>& triangles, const Colorf& color)
{
	if (triangles.empty()) {
		return;
	}

	PrimitiveDrawNew::SetColor(color);
	PrimitiveDrawNew::Draw(GL10::GL_TRIANGLES, &triangles[0].x, triangles.size());
}

void PrimitiveDraw::cross(const Vector& center, float edge, const Colorf& color, float size)
{
	cross(center, edge, edge, color, size);
}

void PrimitiveDraw::cross(const Vector& center, float xedge, float yedge, const Colorf& color, float size)
{
	Vector s = center, e = center;
	s.x -= xedge;
	e.x += xedge;
	drawLine(s, e, color, size);

	s  = e = center;
	s.y -= yedge;
	e.y += yedge;
	drawLine(s, e, color, size);
}

void PrimitiveDraw::text(const char* text)
{
	ISymbol* s = SymbolMgr::Instance()->fetchSymbol(Font::DEFAULT_FONTFILE);
	if (s) {
		ShaderNew::Instance()->null();
		FontSymbol* fs = static_cast<FontSymbol*>(s);
		fs->print(0, 0, text);
	}
}

} // d2d
#include "Box2DRender.h"

#include <sprite2/RVG.h>

namespace ee
{

Box2DRender::Box2DRender(float ratio)
	: m_ratio(ratio)
{
}

void Box2DRender::DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	CU_VEC<sm::vec2> _vertices;
	TransVertices(vertices, vertexCount, _vertices);

	s2::RVG::SetColor(TransColor(color));
	s2::RVG::LineWidth(1);
	s2::RVG::Polyline(nullptr, _vertices, true);
}

void Box2DRender::DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	CU_VEC<sm::vec2> _vertices;
	TransVertices(vertices, vertexCount, _vertices);

	// todo draw with triangles
//	s2::RVG::Polygon(_vertices, TransColor(color, 0.5f));

	s2::RVG::SetColor(TransColor(color));
	s2::RVG::LineWidth(1);
	s2::RVG::Polyline(nullptr, _vertices, true);
}

void Box2DRender::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	s2::RVG::SetColor(TransColor(color));
	s2::RVG::Circle(nullptr, TransVertex(center), radius * m_ratio, true);
}

void Box2DRender::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	sm::vec2 _center = TransVertex(center);
	float _radius = radius * m_ratio;
	s2::RVG::SetColor(TransColor(color, 0.5f));
	s2::RVG::Circle(nullptr, _center, _radius, true);
	s2::RVG::SetColor(TransColor(color));
	s2::RVG::Circle(nullptr, _center, _radius, false);

	DrawSegment(center, center+radius*axis, color);
}

void Box2DRender::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	s2::RVG::SetColor(TransColor(color));
	s2::RVG::LineWidth(1);
	s2::RVG::Line(nullptr, TransVertex(p1), TransVertex(p2));
}

void Box2DRender::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;

	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, b2Color(1,0,0));

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, b2Color(0,1,0));
}

void Box2DRender::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	s2::RVG::SetColor(TransColor(color));
	s2::RVG::PointSize(size);
	s2::RVG::Point(nullptr, TransVertex(p));
}

void Box2DRender::DrawString(int x, int y, const char *string, ...)
{
}

void Box2DRender::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	s2::RVG::SetColor(TransColor(c));
	s2::RVG::LineWidth(1);
	s2::RVG::Rect(nullptr, TransVertex(aabb->lowerBound), TransVertex(aabb->upperBound), false);
}

sm::vec2 Box2DRender::TransVertex(const b2Vec2& vertex) const
{
	return sm::vec2(vertex.x * m_ratio, vertex.y * m_ratio);
}

void Box2DRender::TransVertices(const b2Vec2* src, int count, CU_VEC<sm::vec2>& dst) const
{
	dst.reserve(count);
	for (int i = 0; i < count; ++i) {
		sm::vec2 p(src[i].x, src[i].y);
		p *= m_ratio;
		dst.push_back(p);
	}
}

pt2::Color Box2DRender::TransColor(const b2Color& col, float alpha) const
{
	return pt2::Color((int)(col.r * 255), (int)(col.g * 255), (int)(col.b * 255), (int)(alpha * 255));
}

}
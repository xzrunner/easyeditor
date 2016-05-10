#include "Box2DRender.h"
#include "EE_RVG.h"

namespace ee
{

Box2DRender::Box2DRender(float ratio)
	: m_ratio(ratio)
{
}

void Box2DRender::DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	std::vector<sm::vec2> _vertices;
	TransVertices(vertices, vertexCount, _vertices);

	RVG::Color(TransColor(color));
	RVG::LineWidth(1);
	RVG::Polyline(_vertices, true);
}

void Box2DRender::DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	std::vector<sm::vec2> _vertices;
	TransVertices(vertices, vertexCount, _vertices);

	// todo draw with triangles
//	RVG::Polygon(_vertices, TransColor(color, 0.5f));

	RVG::Color(TransColor(color));
	RVG::LineWidth(1);
	RVG::Polyline(_vertices, true);
}

void Box2DRender::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	RVG::Color(TransColor(color));
	RVG::Circle(TransVertex(center), radius * m_ratio, true);
}

void Box2DRender::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	sm::vec2 _center = TransVertex(center);
	float _radius = radius * m_ratio;
	RVG::Color(TransColor(color, 0.5f));
	RVG::Circle(_center, _radius, true);
	RVG::Color(TransColor(color));
	RVG::Circle(_center, _radius, false);

	DrawSegment(center, center+radius*axis, color);
}

void Box2DRender::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	RVG::Color(TransColor(color));
	RVG::LineWidth(1);
	RVG::Line(TransVertex(p1), TransVertex(p2));
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
	RVG::Color(TransColor(color));
	RVG::PointSize(size);
	RVG::Point(TransVertex(p));
}

void Box2DRender::DrawString(int x, int y, const char *string, ...)
{
}

void Box2DRender::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	RVG::Color(TransColor(c));
	RVG::LineWidth(1);
	RVG::Rect(TransVertex(aabb->lowerBound), TransVertex(aabb->upperBound), false);
}

sm::vec2 Box2DRender::TransVertex(const b2Vec2& vertex) const
{
	return sm::vec2(vertex.x * m_ratio, vertex.y * m_ratio);
}

void Box2DRender::TransVertices(const b2Vec2* src, int count, std::vector<sm::vec2>& dst) const
{
	dst.reserve(count);
	for (int i = 0; i < count; ++i) {
		sm::vec2 p(src[i].x, src[i].y);
		p *= m_ratio;
		dst.push_back(p);
	}
}

s2::Color Box2DRender::TransColor(const b2Color& col, float alpha) const
{
	return s2::Color((int)(col.r * 255), (int)(col.g * 255), (int)(col.b * 255), (int)(alpha * 255));
}

}
#include "Box2DRender.h"

namespace ephysics
{

Box2DRender::Box2DRender(float ratio)
	: m_ratio(ratio)
{
}

void Box2DRender::DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	std::vector<ee::Vector> _vertices;
	TransVertices(vertices, vertexCount, _vertices);
	ee::PrimitiveDraw::DrawPolyline(_vertices, TransColor(color), true, 1);
}

void Box2DRender::DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
{
	std::vector<ee::Vector> _vertices;
	TransVertices(vertices, vertexCount, _vertices);
	ee::PrimitiveDraw::DrawPolygon(_vertices, TransColor(color, 0.5f));
	ee::PrimitiveDraw::DrawPolyline(_vertices, TransColor(color), true, 1);
}

void Box2DRender::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	ee::PrimitiveDraw::DrawCircle(TransVertex(center), radius * m_ratio, true, 1, TransColor(color));
}

void Box2DRender::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	ee::Vector _center = TransVertex(center);
	float _radius = radius * m_ratio;
	ee::PrimitiveDraw::DrawCircle(_center, _radius, true, 1, TransColor(color, 0.5f));
	ee::PrimitiveDraw::DrawCircle(_center, _radius, false, 1, TransColor(color));

	DrawSegment(center, center+radius*axis, color);
}

void Box2DRender::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	ee::PrimitiveDraw::DrawLine(TransVertex(p1), TransVertex(p2), TransColor(color), 1);
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
	ee::PrimitiveDraw::DrawPoint(TransVertex(p), TransColor(color), size);
}

void Box2DRender::DrawString(int x, int y, const char *string, ...)
{
}

void Box2DRender::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	ee::ShapeStyle style;
	style.fill = false;
	style.color = TransColor(c);
	style.size = 1;
	ee::PrimitiveDraw::DrawRect(TransVertex(aabb->lowerBound), TransVertex(aabb->upperBound), style);
}

ee::Vector Box2DRender::TransVertex(const b2Vec2& vertex) const
{
	return ee::Vector(vertex.x * m_ratio, vertex.y * m_ratio);
}

void Box2DRender::TransVertices(const b2Vec2* src, int count, std::vector<ee::Vector>& dst) const
{
	dst.reserve(count);
	for (int i = 0; i < count; ++i) {
		ee::Vector p(src[i].x, src[i].y);
		p *= m_ratio;
		dst.push_back(p);
	}
}

ee::Colorf Box2DRender::TransColor(const b2Color& col, float alpha) const
{
	return ee::Colorf(col.r, col.g, col.b, alpha);
}

}
#ifndef _EPHYSICS_BOX2D_RENDER_H_
#define _EPHYSICS_BOX2D_RENDER_H_

#include <Box2D/Box2D.h>
#include <drag2d.h>

namespace ephysics
{

class Box2DRender : public b2Draw
{
public:
	Box2DRender(float ratio);

	virtual void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);

	virtual void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);

	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	virtual void DrawTransform(const b2Transform& xf);

	virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

	virtual void DrawString(int x, int y, const char* string, ...); 

	virtual void DrawAABB(b2AABB* aabb, const b2Color& color);

private:
	d2d::Vector TransVertex(const b2Vec2& vertex) const;

	void TransVertices(const b2Vec2* src, int count, std::vector<d2d::Vector>& dst) const;

	d2d::Colorf TransColor(const b2Color& col, float alpha = 1) const;

private:
	float m_ratio;

}; // Box2DRender

}

#endif // _EPHYSICS_BOX2D_RENDER_H_
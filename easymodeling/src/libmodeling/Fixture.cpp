
#include "Fixture.h"
#include "Body.h"

#include <easyshape.h>

using namespace libmodeling;

Fixture::Fixture()
	: body(NULL)
	, shape(NULL)
	, density(1.0f)
	, friction(0.2f)
	, restitution(0.0f)
	, isSensor(false)
	, categoryBits(0x0001)
	, maskBits(0xFFFF)
	, groupIndex(0)
{
	static int count = 0;
	name = wxT("fixture") + wxString::FromDouble(count++);
}

Fixture::~Fixture()
{
	shape->Release();
}

bool Fixture::isContain(const d2d::Vector& pos) const
{
	if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shape))
	{
		return d2d::Math2D::GetDistance(circle->center + body->sprite->GetPosition(), pos) 
			< circle->radius;
	}
	else if (libshape::RectShape* rect = dynamic_cast<libshape::RectShape*>(shape))
	{
		std::vector<d2d::Vector> boundary(4);
		boundary[0].Set(rect->m_rect.xmin, rect->m_rect.ymin);
		boundary[1].Set(rect->m_rect.xmax, rect->m_rect.ymin);
		boundary[2].Set(rect->m_rect.xmax, rect->m_rect.ymax);
		boundary[3].Set(rect->m_rect.xmin, rect->m_rect.ymax);

		std::vector<d2d::Vector> fixed;
		transLocalToWorld(boundary, fixed);
		return d2d::Math2D::IsPointInArea(pos, fixed);
	}
	else if (libshape::PolygonShape* polygon = dynamic_cast<libshape::PolygonShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(polygon->GetVertices(), fixed);
		return d2d::Math2D::IsPointInArea(pos, fixed);
	}
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(chain->GetVertices(), fixed);
		return d2d::Math2D::GetDisPointToPolyline(pos, fixed) < 1;
	}
	else
		return false;
}

bool Fixture::isIntersect(const d2d::Rect& rect) const
{
	if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shape))
	{
		return d2d::Math2D::IsCircleIntersectRect(circle->center + body->sprite->GetPosition(), 
			circle->radius, rect);
	}
	else if (libshape::RectShape* r = dynamic_cast<libshape::RectShape*>(shape))
	{
		std::vector<d2d::Vector> boundary(4);
		boundary[0].Set(r->m_rect.xmin, r->m_rect.ymin);
		boundary[1].Set(r->m_rect.xmax, r->m_rect.ymin);
		boundary[2].Set(r->m_rect.xmax, r->m_rect.ymax);
		boundary[3].Set(r->m_rect.xmin, r->m_rect.ymax);
		
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(boundary, fixed);
		return d2d::Math2D::IsPolylineIntersectRect(fixed, true, rect);
	}
	else if (libshape::PolygonShape* polygon = dynamic_cast<libshape::PolygonShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(polygon->GetVertices(), fixed);
		return d2d::Math2D::IsPolylineIntersectRect(fixed, true, rect);
	}
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(chain->GetVertices(), fixed);
		return d2d::Math2D::IsPolylineIntersectRect(fixed, false, rect);
	}
	else
		return false;
}

void Fixture::draw(const d2d::Matrix& mt, const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const
{
	if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shape))
	{
		d2d::PrimitiveDraw::DrawCircle(mt, circle->center, circle->radius, true, 2, cFace);
		d2d::PrimitiveDraw::DrawCircle(mt, circle->center, circle->radius, false, 2, cEdge, 32);
	}
	else if (libshape::RectShape* rect = dynamic_cast<libshape::RectShape*>(shape))
	{
		const d2d::Vector p0(rect->m_rect.xmin, rect->m_rect.ymin),
			p1(rect->m_rect.xmax, rect->m_rect.ymax);
		d2d::PrimitiveDraw::DrawRect(mt, p0, p1, d2d::ShapeStyle(true, cFace));
		d2d::PrimitiveDraw::DrawRect(mt, p0, p1, d2d::ShapeStyle(false, cFace));
	}
	else if (libshape::PolygonShape* polygon = dynamic_cast<libshape::PolygonShape*>(shape))
	{
		std::vector<d2d::Vector> vertices;
		d2d::Math2D::TransVertices(mt, polygon->GetVertices(), vertices);
		d2d::PrimitiveDraw::DrawPolygon(vertices, cFace);
		d2d::PrimitiveDraw::DrawPolyline(vertices, cEdge, true, 2);
	}
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> vertices;
		d2d::Math2D::TransVertices(mt, chain->GetVertices(), vertices);
		d2d::PrimitiveDraw::DrawPolyline(vertices, cEdge, chain->IsClosed(), 2);
	}
}

void Fixture::transLocalToWorld(const std::vector<d2d::Vector>& local, 
								std::vector<d2d::Vector>& world) const
{
	world.resize(local.size());
	for (size_t i = 0, n = local.size(); i < n ; ++i)
		world[i] = d2d::Math2D::RotateVector(local[i], body->sprite->GetAngle()) + body->sprite->GetPosition();
}

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

bool Fixture::isContain(const ee::Vector& pos) const
{
	if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(shape))
	{
		return ee::Math2D::GetDistance(circle->center + body->sprite->GetPosition(), pos) 
			< circle->radius;
	}
	else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(shape))
	{
		std::vector<ee::Vector> boundary(4);
		boundary[0].Set(rect->m_rect.xmin, rect->m_rect.ymin);
		boundary[1].Set(rect->m_rect.xmax, rect->m_rect.ymin);
		boundary[2].Set(rect->m_rect.xmax, rect->m_rect.ymax);
		boundary[3].Set(rect->m_rect.xmin, rect->m_rect.ymax);

		std::vector<ee::Vector> fixed;
		transLocalToWorld(boundary, fixed);
		return ee::Math2D::IsPointInArea(pos, fixed);
	}
	else if (eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(shape))
	{
		std::vector<ee::Vector> fixed;
		transLocalToWorld(polygon->GetVertices(), fixed);
		return ee::Math2D::IsPointInArea(pos, fixed);
	}
	else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(shape))
	{
		std::vector<ee::Vector> fixed;
		transLocalToWorld(chain->GetVertices(), fixed);
		return ee::Math2D::GetDisPointToPolyline(pos, fixed) < 1;
	}
	else
		return false;
}

bool Fixture::isIntersect(const ee::Rect& rect) const
{
	if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(shape))
	{
		return ee::Math2D::IsCircleIntersectRect(circle->center + body->sprite->GetPosition(), 
			circle->radius, rect);
	}
	else if (eshape::RectShape* r = dynamic_cast<eshape::RectShape*>(shape))
	{
		std::vector<ee::Vector> boundary(4);
		boundary[0].Set(r->m_rect.xmin, r->m_rect.ymin);
		boundary[1].Set(r->m_rect.xmax, r->m_rect.ymin);
		boundary[2].Set(r->m_rect.xmax, r->m_rect.ymax);
		boundary[3].Set(r->m_rect.xmin, r->m_rect.ymax);
		
		std::vector<ee::Vector> fixed;
		transLocalToWorld(boundary, fixed);
		return ee::Math2D::IsPolylineIntersectRect(fixed, true, rect);
	}
	else if (eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(shape))
	{
		std::vector<ee::Vector> fixed;
		transLocalToWorld(polygon->GetVertices(), fixed);
		return ee::Math2D::IsPolylineIntersectRect(fixed, true, rect);
	}
	else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(shape))
	{
		std::vector<ee::Vector> fixed;
		transLocalToWorld(chain->GetVertices(), fixed);
		return ee::Math2D::IsPolylineIntersectRect(fixed, false, rect);
	}
	else
		return false;
}

void Fixture::draw(const ee::Matrix& mt, const ee::Colorf& cFace, const ee::Colorf& cEdge) const
{
	if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(shape))
	{
		ee::PrimitiveDraw::DrawCircle(mt, circle->center, circle->radius, true, 2, cFace);
		ee::PrimitiveDraw::DrawCircle(mt, circle->center, circle->radius, false, 2, cEdge, 32);
	}
	else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(shape))
	{
		const ee::Vector p0(rect->m_rect.xmin, rect->m_rect.ymin),
			p1(rect->m_rect.xmax, rect->m_rect.ymax);
		ee::PrimitiveDraw::DrawRect(mt, p0, p1, ee::ShapeStyle(true, cFace));
		ee::PrimitiveDraw::DrawRect(mt, p0, p1, ee::ShapeStyle(false, cFace));
	}
	else if (eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(shape))
	{
		std::vector<ee::Vector> vertices;
		ee::Math2D::TransVertices(mt, polygon->GetVertices(), vertices);
		ee::PrimitiveDraw::DrawPolygon(vertices, cFace);
		ee::PrimitiveDraw::DrawPolyline(vertices, cEdge, true, 2);
	}
	else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(shape))
	{
		std::vector<ee::Vector> vertices;
		ee::Math2D::TransVertices(mt, chain->GetVertices(), vertices);
		ee::PrimitiveDraw::DrawPolyline(vertices, cEdge, chain->IsClosed(), 2);
	}
}

void Fixture::transLocalToWorld(const std::vector<ee::Vector>& local, 
								std::vector<ee::Vector>& world) const
{
	world.resize(local.size());
	for (size_t i = 0, n = local.size(); i < n ; ++i)
		world[i] = ee::Math2D::RotateVector(local[i], body->sprite->GetAngle()) + body->sprite->GetPosition();
}
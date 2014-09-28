
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
	shape->release();
}

bool Fixture::isContain(const d2d::Vector& pos) const
{
	if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shape))
	{
		return d2d::Math::getDistance(circle->center + body->sprite->getPosition(), pos) 
			< circle->radius;
	}
	else if (libshape::RectShape* rect = dynamic_cast<libshape::RectShape*>(shape))
	{
		std::vector<d2d::Vector> boundary(4);
		boundary[0].set(rect->m_rect.xMin, rect->m_rect.yMin);
		boundary[1].set(rect->m_rect.xMax, rect->m_rect.yMin);
		boundary[2].set(rect->m_rect.xMax, rect->m_rect.yMax);
		boundary[3].set(rect->m_rect.xMin, rect->m_rect.yMax);

		std::vector<d2d::Vector> fixed;
		transLocalToWorld(boundary, fixed);
		return d2d::Math::isPointInArea(pos, fixed);
	}
	else if (libshape::PolygonShape* polygon = dynamic_cast<libshape::PolygonShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(polygon->getVertices(), fixed);
		return d2d::Math::isPointInArea(pos, fixed);
	}
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(chain->getVertices(), fixed);
		return d2d::Math::getDisPointToPolyline(pos, fixed) < 1;
	}
	else
		return false;
}

bool Fixture::isIntersect(const d2d::Rect& rect) const
{
	if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shape))
	{
		return d2d::Math::isCircleIntersectRect(circle->center + body->sprite->getPosition(), 
			circle->radius, rect);
	}
	else if (libshape::RectShape* r = dynamic_cast<libshape::RectShape*>(shape))
	{
		std::vector<d2d::Vector> boundary(4);
		boundary[0].set(r->m_rect.xMin, r->m_rect.yMin);
		boundary[1].set(r->m_rect.xMax, r->m_rect.yMin);
		boundary[2].set(r->m_rect.xMax, r->m_rect.yMax);
		boundary[3].set(r->m_rect.xMin, r->m_rect.yMax);
		
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(boundary, fixed);
		return d2d::Math::isPolylineIntersectRect(fixed, true, rect);
	}
	else if (libshape::PolygonShape* polygon = dynamic_cast<libshape::PolygonShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(polygon->getVertices(), fixed);
		return d2d::Math::isPolylineIntersectRect(fixed, true, rect);
	}
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> fixed;
		transLocalToWorld(chain->getVertices(), fixed);
		return d2d::Math::isPolylineIntersectRect(fixed, false, rect);
	}
	else
		return false;
}

void Fixture::draw(const d2d::Screen& scr, const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const
{
	if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shape))
	{
		d2d::PrimitiveDraw::drawCircle(circle->center, circle->radius, true, 2, cFace);
		d2d::PrimitiveDraw::drawCircle(circle->center, circle->radius, false, 2, cEdge, 32);
	}
	else if (libshape::RectShape* rect = dynamic_cast<libshape::RectShape*>(shape))
	{
		const d2d::Vector p0(rect->m_rect.xMin, rect->m_rect.yMin),
			p1(rect->m_rect.xMax, rect->m_rect.yMax);
		d2d::PrimitiveDraw::rect(p0, p1, d2d::ShapeStyle(true, cFace));
		d2d::PrimitiveDraw::rect(p0, p1, d2d::ShapeStyle(false, cFace));
	}
	else if (libshape::PolygonShape* polygon = dynamic_cast<libshape::PolygonShape*>(shape))
	{
		const std::vector<d2d::Vector>& vertices = polygon->getVertices();
		d2d::PrimitiveDraw::drawPolygon(scr, vertices, cFace);
		d2d::PrimitiveDraw::drawPolyline(vertices, cEdge, true, 2);
	}
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
	{
		const std::vector<d2d::Vector>& vertices = chain->getVertices();
		d2d::PrimitiveDraw::drawPolyline(vertices, cEdge, chain->isClosed(), 2);
	}
}

void Fixture::transLocalToWorld(const std::vector<d2d::Vector>& local, 
								std::vector<d2d::Vector>& world) const
{
	world.resize(local.size());
	for (size_t i = 0, n = local.size(); i < n ; ++i)
		world[i] = d2d::Math::rotateVector(local[i], body->sprite->getAngle()) + body->sprite->getPosition();
}
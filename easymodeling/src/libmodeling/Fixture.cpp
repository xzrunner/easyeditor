#include "Fixture.h"
#include "Body.h"

#include <easyshape.h>

#include <ee/StringHelper.h>
#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace emodeling
{

Fixture::Fixture()
	: m_body(NULL)
	, m_shape(NULL)
	, m_density(1.0f)
	, m_friction(0.2f)
	, m_restitution(0.0f)
	, m_is_sensor(false)
	, m_category_bits(0x0001)
	, m_mask_bits(0xFFFF)
	, m_group_index(0)
{
	static int count = 0;
	m_name = std::string("fixture") + ee::StringHelper::ToString(count++);
}

Fixture::~Fixture()
{
	m_shape->Release();
}

bool Fixture::IsContain(const ee::Vector& pos) const
{
	if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(m_shape))
	{
		return ee::Math2D::GetDistance(circle->center + m_body->m_sprite->GetPosition(), pos) 
			< circle->radius;
	}
	else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(m_shape))
	{
		std::vector<ee::Vector> boundary(4);
		boundary[0].Set(rect->m_rect.xmin, rect->m_rect.ymin);
		boundary[1].Set(rect->m_rect.xmax, rect->m_rect.ymin);
		boundary[2].Set(rect->m_rect.xmax, rect->m_rect.ymax);
		boundary[3].Set(rect->m_rect.xmin, rect->m_rect.ymax);

		std::vector<ee::Vector> fixed;
		TransLocalToWorld(boundary, fixed);
		return ee::Math2D::IsPointInArea(pos, fixed);
	}
	else if (eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(m_shape))
	{
		std::vector<ee::Vector> fixed;
		TransLocalToWorld(polygon->GetVertices(), fixed);
		return ee::Math2D::IsPointInArea(pos, fixed);
	}
	else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(m_shape))
	{
		std::vector<ee::Vector> fixed;
		TransLocalToWorld(chain->GetVertices(), fixed);
		return ee::Math2D::GetDisPointToPolyline(pos, fixed) < 1;
	}
	else
		return false;
}

bool Fixture::IsIntersect(const ee::Rect& rect) const
{
	if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(m_shape))
	{
		return ee::Math2D::IsCircleIntersectRect(circle->center + m_body->m_sprite->GetPosition(), 
			circle->radius, rect);
	}
	else if (eshape::RectShape* r = dynamic_cast<eshape::RectShape*>(m_shape))
	{
		std::vector<ee::Vector> boundary(4);
		boundary[0].Set(r->m_rect.xmin, r->m_rect.ymin);
		boundary[1].Set(r->m_rect.xmax, r->m_rect.ymin);
		boundary[2].Set(r->m_rect.xmax, r->m_rect.ymax);
		boundary[3].Set(r->m_rect.xmin, r->m_rect.ymax);
		
		std::vector<ee::Vector> fixed;
		TransLocalToWorld(boundary, fixed);
		return ee::Math2D::IsPolylineIntersectRect(fixed, true, rect);
	}
	else if (eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(m_shape))
	{
		std::vector<ee::Vector> fixed;
		TransLocalToWorld(polygon->GetVertices(), fixed);
		return ee::Math2D::IsPolylineIntersectRect(fixed, true, rect);
	}
	else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(m_shape))
	{
		std::vector<ee::Vector> fixed;
		TransLocalToWorld(chain->GetVertices(), fixed);
		return ee::Math2D::IsPolylineIntersectRect(fixed, false, rect);
	}
	else
		return false;
}

void Fixture::Draw(const ee::Matrix& mt, const ee::Colorf& cFace, const ee::Colorf& cEdge) const
{
	if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(m_shape))
	{
		ee::Vector c = ee::Math2D::TransVector(circle->center, mt);
		float r = ee::Math2D::TransLen(circle->radius, mt);
		ee::RVG::Circle(c, r, true, cFace);
		ee::RVG::Circle(c, r, false, cEdge, 32);
	}
	else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(m_shape))
	{
		ee::Vector min(rect->m_rect.xmin, rect->m_rect.ymin),
			max(rect->m_rect.xmax, rect->m_rect.ymax);
		min = ee::Math2D::TransVector(min, mt);
		max = ee::Math2D::TransVector(max, mt);
		ee::RVG::Rect(min, max, ee::ShapeStyle(true, 2, cFace));
		ee::RVG::Rect(min, max, ee::ShapeStyle(false, 2, cFace));
	}
	else if (eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(m_shape))
	{
		std::vector<ee::Vector> vertices;
		ee::Math2D::TransVertices(mt, polygon->GetVertices(), vertices);
		// todo draw with triangles
//		ee::PrimitiveDraw::DrawPolygon(vertices, cFace);
		ee::RVG::Polyline(vertices, cEdge, true, 2);
	}
	else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(m_shape))
	{
		std::vector<ee::Vector> vertices;
		ee::Math2D::TransVertices(mt, chain->GetVertices(), vertices);
		ee::RVG::Polyline(vertices, cEdge, chain->IsClosed(), 2);
	}
}

void Fixture::TransLocalToWorld(const std::vector<ee::Vector>& local, 
								std::vector<ee::Vector>& world) const
{
	world.resize(local.size());
	for (size_t i = 0, n = local.size(); i < n ; ++i)
		world[i] = ee::Math2D::RotateVector(local[i], m_body->m_sprite->GetAngle()) + m_body->m_sprite->GetPosition();
}

}
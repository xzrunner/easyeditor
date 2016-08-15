#include "PolylineShape.h"
#include "ShapeConfig.h"
#include "ShapeVertices.h"

#include <SM_Test.h>
#include <SM_Calc.h>

namespace s2
{

PolylineShape::PolylineShape(const PolylineShape& polyline)
	: m_vertices(polyline.m_vertices)
	, m_closed(polyline.m_closed)
{
	UpdateBounding();
}

PolylineShape::PolylineShape(const std::vector<sm::vec2>& vertices, bool closed)
	: m_vertices(vertices)
	, m_closed(closed)
{
	UpdateBounding();
}

PolylineShape* PolylineShape::Clone() const
{
	return new PolylineShape(*this);
}

bool PolylineShape::IsContain(const sm::vec2& pos) const
{
	if (m_vertices.empty()) {
		return false;
	}

	if (m_closed) 
	{
		return sm::is_point_in_area(pos, m_vertices);
	}
	else
	{
		sm::rect rect(m_bounding);
		rect.xmin -= SHAPE_NODE_RADIUS;
		rect.xmax += SHAPE_NODE_RADIUS;
		rect.ymin -= SHAPE_NODE_RADIUS;
		rect.ymax += SHAPE_NODE_RADIUS;
		if (!sm::is_point_in_rect(pos, rect)) {
			return false;
		}

		float dis = sm::dis_pos_to_multi_pos(pos, m_vertices);
		if (dis < SHAPE_NODE_RADIUS) {
			return true;
		}

		if (m_vertices.size() < 2) {
			return false;
		}
		for (int i = 0, n = m_vertices.size() - 1; i < n; ++i)
		{
			float dis = sm::dis_pos_to_seg(pos, m_vertices[i], m_vertices[i + 1]);
			if (dis < SHAPE_NODE_RADIUS) {
				return true;
			}
		}

		return false;
	}
}

bool PolylineShape::IsIntersect(const sm::rect& rect) const
{
	if (m_vertices.empty() || !sm::is_rect_intersect_rect(rect, m_bounding)) {
		return false;
	}

	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		if (sm::is_point_in_rect(m_vertices[i], rect))
			return true;
	}

	if (m_vertices.size() < 2) {
		return false;
	}
	for (int i = 0, n = m_vertices.size() - 1; i < n; ++i) {
		if (sm::is_rect_intersect_segment(rect, m_vertices[i], m_vertices[i + 1])) {
			return true;
		}
	}

	if (m_closed && sm::is_rect_intersect_segment(rect, m_vertices.front(), m_vertices.back())) {
		return true;
	}

	return false;
}

void PolylineShape::Draw(const sm::mat4& mt, const RenderColor& color) const
{
	ShapeVertices::Draw(m_vertices, m_closed, mt, color);
}

void PolylineShape::UpdateBounding()
{
	m_bounding.MakeEmpty();
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		m_bounding.Combine(m_vertices[i]);
	}
}

}
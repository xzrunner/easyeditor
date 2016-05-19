#include "PointShape.h"
#include "ShapeConfig.h"
#include "S2_RVG.h"

#include <SM_Test.h>

namespace s2
{

PointShape::PointShape(const sm::vec2& pos)
	: m_pos(pos)
{
	float h = SHAPE_NODE_RADIUS * 0.5f;
	m_region.xmin = m_region.ymin = -h;
	m_region.xmax = m_region.ymax = h;
}

bool PointShape::IsContain(const sm::vec2& pos) const
{
	return sm::is_point_in_rect(pos, m_region);	
}

bool PointShape::IsIntersect(const sm::rect& rect) const
{
	return sm::is_rect_intersect_rect(rect, m_region);
}

void PointShape::Draw(const sm::mat4& mt, const RenderColor& color) const
{
	sm::vec2 center = mt * m_pos;
	float r = sm::mat_trans_len(SHAPE_NODE_RADIUS, mt);
	RVG::Color(color.mul);
	RVG::Circle(center, r, true);
}

}
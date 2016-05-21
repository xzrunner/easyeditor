#include "RectShape.h"
#include "S2_RVG.h"

#include <SM_Test.h>

namespace s2
{

RectShape::RectShape(const sm::rect& r)
{
	m_bounding = r;
}

bool RectShape::IsContain(const sm::vec2& pos) const
{
	return sm::is_point_in_rect(pos, m_bounding);
}

bool RectShape::IsIntersect(const sm::rect& rect) const
{
	return sm::is_rect_intersect_rect(rect, m_bounding);
}

void RectShape::Draw(const sm::mat4& mt, const RenderColor& color) const
{
	sm::vec2 min(m_bounding.xmin, m_bounding.ymin),
		     max(m_bounding.xmax, m_bounding.ymax);
	RVG::Rect(min, max, false);
}

}
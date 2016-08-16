#include "RectQueryVisitor.h"

#include <sprite2/BoundingBox.h>

namespace ee
{

RectQueryVisitor::RectQueryVisitor(const sm::rect& rect, bool contain, std::vector<Sprite*>& result)
	: m_rect(rect)
	, m_contain(contain)
	, m_result(result)
{
}

void RectQueryVisitor::Visit(Sprite* spr, bool& next)
{
// 	if (spr->editable && spr->isIntersect(m_rect))
// 		m_result.push_back(spr);
	if (spr->IsEditable())
	{
		const s2::BoundingBox* bv = spr->GetBounding();
// 		if (!m_contain && bv->isIntersect(m_rect))
// 			m_result.push_back(spr);

		if (m_contain && bv && bv->IsContain(m_rect)) {
			m_result.push_back(spr);
		} else if (!m_contain && bv && bv->IsIntersect(m_rect)) {
			m_result.push_back(spr);
		}
	}
	next = true;
}

}
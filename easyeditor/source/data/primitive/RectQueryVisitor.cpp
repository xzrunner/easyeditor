#include "RectQueryVisitor.h"

#include <SM_Test.h>
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
	next = true;
	if (!spr->IsEditable()) {
		return;
	}
	const s2::BoundingBox* bv = spr->GetBounding();
	if (!bv) {
		return;
	}
	if (m_contain && sm::is_rect_contain_rect(m_rect, bv->GetSize())) {
		m_result.push_back(spr);
	} else if (!m_contain && bv->IsIntersect(m_rect)) {
		m_result.push_back(spr);
	}
}

}
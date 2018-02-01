#include "RectQueryVisitor.h"

#include <SM_Test.h>

namespace ee
{

RectQueryVisitor::RectQueryVisitor(const sm::rect& rect, bool contain)
	: m_rect(rect)
	, m_contain(contain)
{
}

void RectQueryVisitor::Visit(const SprPtr& spr, bool& next)
{
	next = true;
	if (!spr->IsEditable()) {
		return;
	}
	auto& bb = spr->GetBounding();
	if (m_contain && sm::is_rect_contain_rect(m_rect, bb.GetSize())) {
		m_selected.push_back(spr);
	} else if (!m_contain && bb.IsIntersect(m_rect)) {
		m_selected.push_back(spr);
	}
}

}
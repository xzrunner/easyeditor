#ifndef _EASYEDITOR_RECT_QUERY_VISITOR_H_
#define _EASYEDITOR_RECT_QUERY_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"
#include "BoundingBox.h"

#include <vector>

namespace ee
{

class RectQueryVisitor : public Visitor<Sprite>
{
public:
	RectQueryVisitor(const sm::rect& rect, bool contain, std::vector<Sprite*>& result);
	virtual void Visit(Sprite* spr, bool& next);

private:
	const sm::rect& m_rect;
	bool m_contain;
	std::vector<Sprite*>& m_result;

}; // RectQueryVisitor

inline
RectQueryVisitor::RectQueryVisitor(const sm::rect& rect, bool contain, std::vector<Sprite*>& result)
	: m_rect(rect)
	, m_contain(contain)
	, m_result(result)
{
}

inline
void RectQueryVisitor::Visit(Sprite* spr, bool& next)
{
// 	if (spr->editable && spr->isIntersect(m_rect))
// 		m_result.push_back(spr);
	if (spr->IsEditable())
	{
		BoundingBox* bv = spr->GetBounding();
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

#endif // _EASYEDITOR_RECT_QUERY_VISITOR_H_
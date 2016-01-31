#ifndef _EASYEDITOR_RECT_QUERY_VISITOR_H_
#define _EASYEDITOR_RECT_QUERY_VISITOR_H_

#include "Visitor.h"
#include "Rect.h"
#include "Sprite.h"
#include "BoundingBox.h"

#include <vector>

namespace ee
{

class RectQueryVisitor : public Visitor
{
public:
	RectQueryVisitor(const Rect& rect, bool contain, std::vector<Sprite*>& result);
	virtual void Visit(Object* object, bool& next);

private:
	const Rect& m_rect;
	bool m_contain;
	std::vector<Sprite*>& m_result;

}; // RectQueryVisitor

inline
RectQueryVisitor::RectQueryVisitor(const Rect& rect, bool contain, std::vector<Sprite*>& result)
	: m_rect(rect)
	, m_contain(contain)
	, m_result(result)
{
}

inline
void RectQueryVisitor::Visit(Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);
// 	if (sprite->editable && sprite->isIntersect(m_rect))
// 		m_result.push_back(sprite);
	if (sprite->editable)
	{
		BoundingBox* bv = sprite->GetBounding();
// 		if (!m_contain && bv->isIntersect(m_rect))
// 			m_result.push_back(sprite);

		if (m_contain && bv && bv->IsContain(m_rect)) {
			m_result.push_back(sprite);
		} else if (!m_contain && bv && bv->IsIntersect(m_rect)) {
			m_result.push_back(sprite);
		}
	}
	next = true;
}

}

#endif // _EASYEDITOR_RECT_QUERY_VISITOR_H_
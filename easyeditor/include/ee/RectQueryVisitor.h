#ifndef _EASYEDITOR_RECT_QUERY_VISITOR_H_
#define _EASYEDITOR_RECT_QUERY_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"

#include <vector>

namespace ee
{

class RectQueryVisitor : public RefVisitor<Sprite>
{
public:
	RectQueryVisitor(const sm::rect& rect, bool contain);
	virtual void Visit(const SprPtr& spr, bool& next);

	const std::vector<SprPtr>& GetSelected() const { return m_selected; }

private:
	const sm::rect& m_rect;
	bool m_contain;
	std::vector<SprPtr> m_selected;

}; // RectQueryVisitor

}

#endif // _EASYEDITOR_RECT_QUERY_VISITOR_H_
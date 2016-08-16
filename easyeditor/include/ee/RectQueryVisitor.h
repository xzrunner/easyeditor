#ifndef _EASYEDITOR_RECT_QUERY_VISITOR_H_
#define _EASYEDITOR_RECT_QUERY_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"

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

}

#endif // _EASYEDITOR_RECT_QUERY_VISITOR_H_
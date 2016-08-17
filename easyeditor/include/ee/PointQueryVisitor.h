#ifndef _EASYEDITOR_POINT_QUERY_VISITOR_H_
#define _EASYEDITOR_POINT_QUERY_VISITOR_H_

#include "Visitor.h"

#include <sprite2/BoundingBox.h>

namespace ee
{

class PointQueryVisitor : public Visitor<Sprite>
{
public:
	PointQueryVisitor(const sm::vec2& pos, Sprite** result);
	virtual ~PointQueryVisitor();
	virtual void Visit(Sprite* spr, bool& next);

private:
	const sm::vec2& m_pos;
	Sprite** m_result;

}; // PointQueryVisitor

inline
PointQueryVisitor::PointQueryVisitor(const sm::vec2& pos, Sprite** result)
	: m_pos(pos)
{
	m_result = result;
	*m_result = NULL;
}

inline 
PointQueryVisitor::~PointQueryVisitor()
{
	if (*m_result) {
		(*m_result)->RemoveReference();
	}
}

inline
void PointQueryVisitor::Visit(Sprite* spr, bool& next)
{
	if (spr->GetBounding()->IsContain(m_pos))
	{
		cu::RefCountObjAssign(*m_result, spr);
		next = false;
	}
	else
	{
		next = true;
	}
}

}

#endif // _EASYEDITOR_POINT_QUERY_VISITOR_H_
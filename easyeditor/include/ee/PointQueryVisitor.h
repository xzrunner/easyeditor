#ifndef _EASYEDITOR_POINT_QUERY_VISITOR_H_
#define _EASYEDITOR_POINT_QUERY_VISITOR_H_

#include "Visitor.h"

namespace ee
{

class PointQueryVisitor : public Visitor
{
public:
	PointQueryVisitor(const Vector& pos, Sprite** result);
	virtual void Visit(Object* object, bool& next);

private:
	const Vector& m_pos;
	Sprite** m_result;

}; // PointQueryVisitor

inline
PointQueryVisitor::PointQueryVisitor(const Vector& pos, Sprite** result)
	: m_pos(pos)
{
	m_result = result;
	*m_result = NULL;
}

inline
void PointQueryVisitor::Visit(Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);
	if (sprite->IsContain(m_pos))
	{
		obj_assign<Sprite>(*m_result, sprite);
		next = false;
	}
	else
	{
		next = true;
	}
}

}

#endif // _EASYEDITOR_POINT_QUERY_VISITOR_H_
#ifndef _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_
#define _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"

#include <sprite2/BoundingBox.h>

namespace ee
{

class Sprite;

class PointMultiQueryVisitor : public Visitor<Sprite>
{
public:
	PointMultiQueryVisitor(const sm::vec2& pos);
	virtual void Visit(Sprite* spr, bool& next);

	const std::vector<Sprite*>& GetResult() const { return m_sprs; }

private:
	const sm::vec2& m_pos;
	std::vector<Sprite*> m_sprs;

}; // PointMultiQueryVisitor

inline
PointMultiQueryVisitor::PointMultiQueryVisitor(const sm::vec2& pos)
	: m_pos(pos)
{
}

inline
void PointMultiQueryVisitor::Visit(Sprite* spr, bool& next)
{
	if (spr->GetBounding()->IsContain(m_pos))
	{
		m_sprs.push_back(spr);
		next = false;
	}

	next = true;
}

}

#endif // _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_
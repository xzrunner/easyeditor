#ifndef _EASYEDITOR_POINT_QUERY_VISITOR_H_
#define _EASYEDITOR_POINT_QUERY_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"

namespace ee
{

class PointQueryVisitor : public RefVisitor<Sprite>
{
public:
	PointQueryVisitor(const sm::vec2& pos);
	virtual void Visit(const SprPtr& spr, bool& next) override;

	const SprPtr& GetSelected() const { return m_selected; }

private:
	const sm::vec2& m_pos;

	SprPtr m_selected;

}; // PointQueryVisitor

inline
PointQueryVisitor::PointQueryVisitor(const sm::vec2& pos)
	: m_pos(pos)
{
}

inline
void PointQueryVisitor::Visit(const SprPtr& spr, bool& next)
{
	if (spr->GetBounding()->IsContain(m_pos))
	{
		m_selected = spr;
		next = false;
	}
	else
	{
		next = true;
	}
}

}

#endif // _EASYEDITOR_POINT_QUERY_VISITOR_H_
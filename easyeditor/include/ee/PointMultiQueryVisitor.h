#ifndef _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_
#define _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"

namespace ee
{

class Sprite;

class PointMultiQueryVisitor : public RefVisitor<Sprite>
{
public:
	PointMultiQueryVisitor(const sm::vec2& pos);
	virtual void Visit(const SprPtr& spr, bool& next) override;

	const std::vector<SprPtr>& GetResult() const { return m_sprs; }

private:
	const sm::vec2& m_pos;
	std::vector<SprPtr> m_sprs;

}; // PointMultiQueryVisitor

inline
PointMultiQueryVisitor::PointMultiQueryVisitor(const sm::vec2& pos)
	: m_pos(pos)
{
}

inline
void PointMultiQueryVisitor::Visit(const SprPtr& spr, bool& next)
{
	if (spr->GetBounding().IsContain(m_pos))
	{
		m_sprs.push_back(spr);
		next = false;
	}

	next = true;
}

}

#endif // _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_
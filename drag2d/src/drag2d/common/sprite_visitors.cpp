#include "sprite_visitors.h"

#include "dataset/ISprite.h"
#include "dataset/AbstractBV.h"

namespace d2d
{

//////////////////////////////////////////////////////////////////////////
// class PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

PointQueryVisitor::PointQueryVisitor(const Vector& pos, ISprite** result)
	: m_pos(pos)
{
	m_result = result;
	*m_result = NULL;
}

void PointQueryVisitor::Visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	if (sprite->IsContain(m_pos))
	{
		*m_result = sprite;
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

RectQueryVisitor::RectQueryVisitor(const Rect& rect, bool contain, std::vector<ISprite*>& result)
	: m_rect(rect)
	, m_contain(contain)
	, m_result(result)
{
}

void RectQueryVisitor::Visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	// 	if (sprite->editable && sprite->isIntersect(m_rect))
	// 		m_result.push_back(sprite);
	if (sprite->editable)
	{
		AbstractBV* bv = sprite->GetBounding();
		// 		if (!m_contain && bv->isIntersect(m_rect))
		// 			m_result.push_back(sprite);

		if (m_contain && bv && bv->isContain(m_rect)) {
			m_result.push_back(sprite);
		} else if (!m_contain && bv && bv->isIntersect(m_rect)) {
			m_result.push_back(sprite);
		}
	}
	bFetchNext = true;
}

}
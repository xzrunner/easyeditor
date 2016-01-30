#include "sprite_visitors.h"

#include "dataset/Sprite.h"
#include "dataset/BoundingBox.h"

namespace d2d
{

//////////////////////////////////////////////////////////////////////////
// class PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

PointQueryVisitor::PointQueryVisitor(const Vector& pos, Sprite** result)
	: m_pos(pos)
{
	m_result = result;
	*m_result = NULL;
}

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

//////////////////////////////////////////////////////////////////////////
// class PointMultiQueryVisitor
//////////////////////////////////////////////////////////////////////////

PointMultiQueryVisitor::PointMultiQueryVisitor(const Vector& pos)
	: m_pos(pos)
{
}

void PointMultiQueryVisitor::Visit(Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);
	if (sprite->IsContain(m_pos))
	{
		m_sprites.push_back(sprite);
		next = false;
	}

	next = true;
}

//////////////////////////////////////////////////////////////////////////
// class RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

RectQueryVisitor::RectQueryVisitor(const Rect& rect, bool contain, std::vector<Sprite*>& result)
	: m_rect(rect)
	, m_contain(contain)
	, m_result(result)
{
}

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
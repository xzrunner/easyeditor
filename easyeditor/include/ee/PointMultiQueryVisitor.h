#ifndef _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_
#define _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_

#include "Visitor.h"
#include "Sprite.h"

namespace ee
{

class Sprite;

class PointMultiQueryVisitor : public Visitor
{
public:
	PointMultiQueryVisitor(const sm::vec2& pos);
	virtual void Visit(Object* object, bool& next);

	const std::vector<Sprite*>& GetResult() const { return m_sprites; }

private:
	const sm::vec2& m_pos;
	std::vector<Sprite*> m_sprites;

}; // PointMultiQueryVisitor

inline
PointMultiQueryVisitor::PointMultiQueryVisitor(const sm::vec2& pos)
	: m_pos(pos)
{
}

inline
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

}

#endif // _EASYEDITOR_POINT_MULTI_QUERY_VISITOR_H_
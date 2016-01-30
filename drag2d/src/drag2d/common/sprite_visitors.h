#ifndef _DRAG2D_SPRITE_VISITORS_H_
#define _DRAG2D_SPRITE_VISITORS_H_

#include "interfaces.h"

namespace d2d
{

class Rect;

class PointQueryVisitor : public Visitor
{
public:
	PointQueryVisitor(const Vector& pos, Sprite** result);
	virtual void Visit(Object* object, bool& next);

private:
	const Vector& m_pos;
	Sprite** m_result;

}; // PointQueryVisitor

class PointMultiQueryVisitor : public Visitor
{
public:
	PointMultiQueryVisitor(const Vector& pos);
	virtual void Visit(Object* object, bool& next);

	const std::vector<Sprite*>& GetResult() const { return m_sprites; }

private:
	const Vector& m_pos;
	std::vector<Sprite*> m_sprites;

}; // PointMultiQueryVisitor

class RectQueryVisitor : public Visitor
{
public:
	RectQueryVisitor(const Rect& rect, bool contain, std::vector<Sprite*>& result);
	virtual void Visit(Object* object, bool& next);

private:
	const Rect& m_rect;
	bool m_contain;
	std::vector<Sprite*>& m_result;

}; // RectQueryVisitor

}

#endif // _DRAG2D_SPRITE_VISITORS_H_
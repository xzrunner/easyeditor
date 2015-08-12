#ifndef _DRAG2D_SPRITE_VISITORS_H_
#define _DRAG2D_SPRITE_VISITORS_H_

#include "interfaces.h"

namespace d2d
{

class Rect;

class PointQueryVisitor : public IVisitor
{
public:
	PointQueryVisitor(const Vector& pos, ISprite** result);
	virtual void Visit(Object* object, bool& bFetchNext);

private:
	const Vector& m_pos;
	ISprite** m_result;

}; // PointQueryVisitor

class PointMultiQueryVisitor : public IVisitor
{
public:
	PointMultiQueryVisitor(const Vector& pos);
	virtual void Visit(Object* object, bool& bFetchNext);

	const std::vector<ISprite*>& GetResult() const { return m_sprites; }

private:
	const Vector& m_pos;
	std::vector<ISprite*> m_sprites;

}; // PointMultiQueryVisitor

class RectQueryVisitor : public IVisitor
{
public:
	RectQueryVisitor(const Rect& rect, bool contain, std::vector<ISprite*>& result);
	virtual void Visit(Object* object, bool& bFetchNext);

private:
	const Rect& m_rect;
	bool m_contain;
	std::vector<ISprite*>& m_result;

}; // RectQueryVisitor

}

#endif // _DRAG2D_SPRITE_VISITORS_H_
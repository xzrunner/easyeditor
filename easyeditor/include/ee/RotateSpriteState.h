#ifndef _EASYEDITOR_ROTATE_SPRITE_STATE_H_
#define _EASYEDITOR_ROTATE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Vector.h"
#include "Visitor.h"

namespace ee
{

class SpriteSelection;

class RotateSpriteState : public ArrangeSpriteState
{
public:
	RotateSpriteState(SpriteSelection* selection, const Vector& first_pos);
	virtual ~RotateSpriteState();

	virtual void OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

protected:
	virtual void Rotate(const Vector& dst);

protected:
	SpriteSelection* GetSelection() { return m_selection; } 

	Vector GetLastPos() const { return m_last_pos; }

private:
	class RotateVisitor : public Visitor
	{
	public:
		RotateVisitor(const Vector& start, const Vector& end) : m_start(start), m_end(end) {}
		virtual void Visit(Object* object, bool& next);
	private:
		Vector m_start, m_end;
	}; // RotateVisitor

protected:
	SpriteSelection* m_selection;

	Vector m_first_pos, m_last_pos;

}; // RotateSpriteState

}

#endif // _EASYEDITOR_ROTATE_SPRITE_STATE_H_
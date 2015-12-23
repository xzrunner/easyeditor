#ifndef _DRAG2D_ROTATE_SPRITE_STATE_H_
#define _DRAG2D_ROTATE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"
#include "interfaces.h"

#include "common/Vector.h"

namespace d2d
{

class SpriteSelection;

class RotateSpriteState : public IArrangeSpriteState
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
	class Visitor : public IVisitor
	{
	public:
		Visitor(const Vector& start, const Vector& end) : m_start(start), m_end(end) {}
		virtual void Visit(Object* object, bool& bFetchNext);
	private:
		Vector m_start, m_end;
	}; // Visitor

protected:
	SpriteSelection* m_selection;

	Vector m_first_pos, m_last_pos;

}; // RotateSpriteState

}

#endif // _DRAG2D_ROTATE_SPRITE_STATE_H_
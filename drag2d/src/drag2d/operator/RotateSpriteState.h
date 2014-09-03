#ifndef _DRAG2D_ROTATE_SPRITE_STATE_H_
#define _DRAG2D_ROTATE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"
#include "dataset/SelectionSet.h"

namespace d2d
{

class RotateSpriteState : public IArrangeSpriteState
{
public:
	RotateSpriteState(SpriteSelection* selection, const Vector& first_pos);
	virtual ~RotateSpriteState();

	virtual AbstractAtomicOP* OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseMove(const Vector& pos);

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
		virtual void visit(Object* object, bool& bFetchNext);
	private:
		Vector m_start, m_end;
	}; // Visitor

private:
	SpriteSelection* m_selection;

	Vector m_first_pos, m_last_pos;

}; // RotateSpriteState

}

#endif // _DRAG2D_ROTATE_SPRITE_STATE_H_
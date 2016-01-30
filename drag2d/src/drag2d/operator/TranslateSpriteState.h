#ifndef _DRAG2D_TRANSLATE_SPRITE_STATE_H_
#define _DRAG2D_TRANSLATE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"
#include "interfaces.h"

#include "common/Vector.h"

namespace d2d
{

class SpriteSelection;

class TranslateSpriteState : public IArrangeSpriteState
{
public:
	TranslateSpriteState(SpriteSelection* selection, const Vector& first_pos);
	virtual ~TranslateSpriteState();

	virtual	void OnMousePress(const Vector& pos);
	virtual void OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

	virtual bool OnDirectionKeyDown(DirectionType type);

protected:
	virtual void Translate(const Vector& offset);

protected:
	SpriteSelection* GetSelection() { return m_selection; } 

private:
	class Visitor : public Visitor
	{
	public:
		Visitor(const Vector& offset) : m_offset(offset) {}
		virtual void Visit(Object* object, bool& next);
	private:
		Vector m_offset;
	}; // Visitor

private:
	SpriteSelection* m_selection;

	Vector m_first_pos, m_last_pos;

	bool m_dirty;
	
}; // TranslateState

}

#endif // _DRAG2D_TRANSLATE_SPRITE_STATE_H_
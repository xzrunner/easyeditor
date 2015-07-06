#ifndef _DRAG2D_TRANSLATE_SPRITE_STATE_H_
#define _DRAG2D_TRANSLATE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class TranslateSpriteState : public IArrangeSpriteState
{
public:
	TranslateSpriteState(SpriteSelection* selection, const Vector& first_pos);
	virtual ~TranslateSpriteState();

	virtual	void OnMousePress(const Vector& pos);
	virtual AbstractAtomicOP* OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

	virtual bool OnDirectionKeyDown(DirectionType type);

protected:
	virtual void Translate(const Vector& offset);

protected:
	SpriteSelection* GetSelection() { return m_selection; } 

private:
	class Visitor : public IVisitor
	{
	public:
		Visitor(const Vector& offset) : m_offset(offset) {}
		virtual void Visit(Object* object, bool& bFetchNext);
	private:
		Vector m_offset;
	}; // Visitor

private:
	SpriteSelection* m_selection;

	Vector m_first_pos, m_last_pos;
	
}; // TranslateState

}

#endif // _DRAG2D_TRANSLATE_SPRITE_STATE_H_
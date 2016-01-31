#ifndef _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_
#define _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Vector.h"
#include "Visitor.h"

namespace ee
{

class SpriteSelection;

class TranslateSpriteState : public ArrangeSpriteState
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
	class TranslateVisitor : public Visitor
	{
	public:
		TranslateVisitor(const Vector& offset) : m_offset(offset) {}
		virtual void Visit(Object* object, bool& next);
	private:
		Vector m_offset;
	}; // TranslateVisitor

private:
	SpriteSelection* m_selection;

	Vector m_first_pos, m_last_pos;

	bool m_dirty;
	
}; // TranslateState

}

#endif // _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_
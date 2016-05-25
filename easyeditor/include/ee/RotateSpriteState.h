#ifndef _EASYEDITOR_ROTATE_SPRITE_STATE_H_
#define _EASYEDITOR_ROTATE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Visitor.h"

namespace ee
{

class SpriteSelection;

class RotateSpriteState : public ArrangeSpriteState
{
public:
	RotateSpriteState(SpriteSelection* selection, const sm::vec2& first_pos);
	virtual ~RotateSpriteState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

protected:
	virtual void Rotate(const sm::vec2& dst);

protected:
	SpriteSelection* GetSelection() { return m_selection; } 

	sm::vec2 GetLastPos() const { return m_last_pos; }

private:
	class RotateVisitor : public Visitor
	{
	public:
		RotateVisitor(const sm::vec2& start, const sm::vec2& end, float& angle) 
			: m_start(start), m_end(end), m_angle(angle) {}
		virtual void Visit(Object* object, bool& next);
	private:
		sm::vec2 m_start, m_end;
		float& m_angle;
	}; // RotateVisitor

protected:
	SpriteSelection* m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	float m_angle;

}; // RotateSpriteState

}

#endif // _EASYEDITOR_ROTATE_SPRITE_STATE_H_
#ifndef _LIBSKETCH_TRANSLATE_SPRITE_STATE_H_
#define _LIBSKETCH_TRANSLATE_SPRITE_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace libsketch
{

class StagePanel;

class TranslateSpriteState : public IEditState
{
public:
	TranslateSpriteState(StagePanel* stage, const d2d::SpriteSelection& selection);

	virtual void OnMousePress(const ivec2& pos);
	virtual void OnMouseRelease(const ivec2& pos);
	virtual void OnMouseMove(const ivec2& pos);	

private:
	void Translate(const ivec2& first, const ivec2& curr);

private:
	class Visitor : public d2d::IVisitor
	{
	public:
		Visitor(StagePanel* stage, const ivec2& last, const ivec2& curr) 
			: m_stage(stage), m_last(last), m_curr(curr) {}
		virtual void visit(d2d::Object* object, bool& bFetchNext);
	private:
		StagePanel* m_stage;
		ivec2 m_last, m_curr;

	}; // Visitor

private:
	StagePanel* m_stage;
	const d2d::SpriteSelection& m_selection;

	ivec2 m_first_pos, m_last_pos;

}; // TranslateSpriteState

}

#endif // _LIBSKETCH_TRANSLATE_SPRITE_STATE_H_
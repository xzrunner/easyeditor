#ifndef _EASYSKETCH_TRANSLATE_SPRITE_STATE_H_
#define _EASYSKETCH_TRANSLATE_SPRITE_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

#include <ee/Visitor.h>

namespace ee { class SpriteSelection; }

namespace esketch
{

class StagePanel;

class TranslateSpriteState : public IEditState
{
public:
	TranslateSpriteState(StagePanel* stage, const ee::SpriteSelection& selection);

	virtual void OnMousePress(const sm::ivec2& pos);
	virtual void OnMouseRelease(const sm::ivec2& pos);
	virtual void OnMouseMove(const sm::ivec2& pos);	

private:
	void Translate(const sm::ivec2& first, const sm::ivec2& curr);

private:
	class Visitor : public ee::Visitor<ee::Sprite>
	{
	public:
		Visitor(StagePanel* stage, const sm::ivec2& last, const sm::ivec2& curr) 
			: m_stage(stage), m_last(last), m_curr(curr) {}
		virtual void Visit(ee::Sprite* spr, bool& next);
	private:
		StagePanel* m_stage;
		sm::ivec2 m_last, m_curr;

	}; // Visitor

private:
	StagePanel* m_stage;
	const ee::SpriteSelection& m_selection;

	sm::ivec2 m_first_pos, m_last_pos;

}; // TranslateSpriteState

}

#endif // _EASYSKETCH_TRANSLATE_SPRITE_STATE_H_
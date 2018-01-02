#ifndef _EASYCOMPLEX3D_TRANSLATE_SPRITE_STATE_H_
#define _EASYCOMPLEX3D_TRANSLATE_SPRITE_STATE_H_

#include "IEditState.h"

#include <ee/Visitor.h>
#include <ee/Sprite.h>

namespace ee { class SpriteSelection; }

namespace ecomplex3d
{

class StagePanel;

class TranslateSpriteState : public IEditState
{
public:
	TranslateSpriteState(StagePanel* stage, const ee::SpriteSelection& selection);

	virtual void OnMousePress(const sm::ivec2& pos) override;
	virtual void OnMouseRelease(const sm::ivec2& pos) override;
	virtual void OnMouseMove(const sm::ivec2& pos) override;	

private:
	void Translate(const sm::ivec2& first, const sm::ivec2& curr);

private:
	class Visitor : public ee::RefVisitor<ee::Sprite>
	{
	public:
		Visitor(StagePanel* stage, const sm::ivec2& last, const sm::ivec2& curr) 
			: m_stage(stage), m_last(last), m_curr(curr) {}
		virtual void Visit(const ee::SprPtr& spr, bool& next) override;
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

#endif // _EASYCOMPLEX3D_TRANSLATE_SPRITE_STATE_H_

#include "TranslateSpriteState.h"
#include "Sprite.h"
#include "StagePanel.h"

namespace libsketch
{

TranslateSpriteState::TranslateSpriteState(StagePanel* stage,
										   const d2d::SpriteSelection& selection)
	: m_stage(stage)
	, m_selection(selection)
{
}

void TranslateSpriteState::OnMousePress(const ivec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void TranslateSpriteState::OnMouseRelease(const ivec2& pos)
{
	// todo history
}

void TranslateSpriteState::OnMouseMove(const ivec2& pos)
{
	if (m_selection.IsEmpty()) {
		return;
	}

	Translate(m_last_pos, pos);
	m_last_pos = pos;

	m_stage->SetCanvasDirty();
}

void TranslateSpriteState::Translate(const ivec2& first, const ivec2& curr)
{
	m_selection.Traverse(Visitor(m_stage, first, curr));
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
Visit(d2d::Object* object, bool& bFetchNext)
{
	Sprite* sprite = static_cast<Sprite*>(object);

	const vec3& old = sprite->GetPos3();
	vec3 last = m_stage->TransPos3ScreenToProject(m_last, old.z);
	vec3 curr = m_stage->TransPos3ScreenToProject(m_curr, old.z);
	sprite->Translate3(curr - last);

	bFetchNext = true;
}

}
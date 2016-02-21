#include "TranslateSpriteState.h"
#include "Sprite.h"
#include "StagePanel.h"

#include <ee/SpriteSelection.h>
#include <ee/panel_msg.h>

namespace esketch
{

TranslateSpriteState::TranslateSpriteState(StagePanel* stage,
										   const ee::SpriteSelection& selection)
	: m_stage(stage)
	, m_selection(selection)
{
}

void TranslateSpriteState::OnMousePress(const ee::ivec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void TranslateSpriteState::OnMouseRelease(const ee::ivec2& pos)
{
	// todo history
}

void TranslateSpriteState::OnMouseMove(const ee::ivec2& pos)
{
	if (m_selection.IsEmpty()) {
		return;
	}

	Translate(m_last_pos, pos);
	m_last_pos = pos;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void TranslateSpriteState::Translate(const ee::ivec2& first, const ee::ivec2& curr)
{
	m_selection.Traverse(Visitor(m_stage, first, curr));
}

//////////////////////////////////////////////////////////////////////////
// class TranslateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void TranslateSpriteState::Visitor::
Visit(ee::Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);

	const ee::vec3& old = sprite->GetPos3();
	ee::vec3 last = m_stage->TransPos3ScreenToProject(m_last, old.z);
	ee::vec3 curr = m_stage->TransPos3ScreenToProject(m_curr, old.z);
	sprite->Translate3(curr - last);

	next = true;
}

}
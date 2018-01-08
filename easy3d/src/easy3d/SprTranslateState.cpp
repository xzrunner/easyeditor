#include "SprTranslateState.h"
#include "StageCanvas.h"

#include <ee/SpriteSelection.h>
#include <ee/panel_msg.h>

#include <sprite2/ModelSprite.h>

namespace e3d
{

SprTranslateState::SprTranslateState(StageCanvas& canvas, const ee::SpriteSelection& selection)
	: m_canvas(canvas)
	, m_selection(selection)
{
}

void SprTranslateState::OnMousePress(const sm::vec2& pos)
{
	m_first_pos = m_last_pos = pos;
}

void SprTranslateState::OnMouseRelease(const sm::vec2& pos)
{
	// todo history
}

void SprTranslateState::OnMouseMove(const sm::vec2& pos)
{
	if (m_selection.IsEmpty()) {
		return;
	}

	Translate(m_last_pos, pos);
	m_last_pos = pos;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SprTranslateState::Translate(const sm::vec2& first, const sm::vec2& curr)
{
	m_selection.Traverse(Visitor(m_canvas, first, curr));
}

//////////////////////////////////////////////////////////////////////////
// class SprTranslateState::Visitor
//////////////////////////////////////////////////////////////////////////

void SprTranslateState::Visitor::
Visit(const ee::SprPtr& spr, bool& next)
{
	auto model_spr = std::dynamic_pointer_cast<s2::ModelSprite>(spr);

	const sm::vec3& old = model_spr->GetPos3();
	sm::vec3 last = m_canvas.TransPos3ScreenToProject(m_last, old.z);
	sm::vec3 curr = m_canvas.TransPos3ScreenToProject(m_curr, old.z);
	model_spr->Translate3(curr - last);

	next = true;
}

}
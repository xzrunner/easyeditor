#include "SprRotateState.h"
#include "StageCanvas.h"

#include <sprite2/ModelSprite.h>

namespace enode3d
{

SprRotateState::SprRotateState(StageCanvas& canvas, const ee::SpriteSelection& selection)
	: m_canvas(canvas)
	, m_selection(selection)
{
}

void SprRotateState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

void SprRotateState::OnMouseRelease(const sm::vec2& pos)
{
}

void SprRotateState::OnMouseDrag(const sm::vec2& pos)
{
	Rotate(m_last_pos, pos);
	m_last_pos = pos;

	m_canvas.Refresh();
}

void SprRotateState::Rotate(const sm::vec2& start, const sm::vec2& end)
{
	m_selection.Traverse(Visitor(m_canvas, start, end));
}

//////////////////////////////////////////////////////////////////////////
// class SprRotateState::Visitor
//////////////////////////////////////////////////////////////////////////

void SprRotateState::Visitor::
Visit(const ee::SprPtr& spr, bool& next)
{
	auto model_spr = std::dynamic_pointer_cast<s2::ModelSprite>(spr);

	sm::vec2 center = m_canvas.TransPos3ProjectToScreen(model_spr->GetPos3());
	sm::vec2 base = m_canvas.TransPos3ProjectToScreen(sm::vec3(0, 0, 0));

	auto& vp = m_canvas.GetViewport();
   	sm::vec3 start = vp.MapToSphere(base + m_start -  center);
   	sm::vec3 end   = vp.MapToSphere(base + m_end - center);
   	sm::Quaternion delta = sm::Quaternion::CreateFromVectors(start, end);

	model_spr->Rotate3(delta);

	next = true;
}

}
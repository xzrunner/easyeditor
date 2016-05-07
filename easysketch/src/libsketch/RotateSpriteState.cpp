#include "RotateSpriteState.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "StageCanvas.h"

namespace esketch
{

RotateSpriteState::RotateSpriteState(StagePanel* stage,
									 const ee::SpriteSelection& selection)
	: m_stage(stage)
	, m_selection(selection)
{
}

void RotateSpriteState::OnMousePress(const sm::ivec2& pos)
{
	m_last_pos = pos;
}

void RotateSpriteState::OnMouseRelease(const sm::ivec2& pos)
{
}

void RotateSpriteState::OnMouseMove(const sm::ivec2& pos)
{
	StageCanvas* canvas = static_cast<StageCanvas*>(m_stage->GetCanvas());

	e3d::Camera& cam = canvas->GetCamera3();
	Rotate(cam, m_last_pos, pos);
	m_last_pos = pos;

	canvas->Refresh();
}

void RotateSpriteState::Rotate(const e3d::Camera& cam, const sm::ivec2& start, const sm::ivec2& end)
{
	m_selection.Traverse(Visitor(m_stage, cam, start, end));
}

//////////////////////////////////////////////////////////////////////////
// class RotateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void RotateSpriteState::Visitor::
Visit(ee::Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);

	sm::ivec2 center = m_stage->TransPos3ProjectToScreen(sprite->GetPos3());
	sm::ivec2 base = m_stage->TransPos3ProjectToScreen(sm::vec3(0, 0, 0));

   	sm::vec3 start = m_cam.MapToSphere(base + m_start -  center);
   	sm::vec3 end = m_cam.MapToSphere(base + m_end - center);
   	sm::Quaternion delta = sm::Quaternion::CreateFromVectors(start, end);

 	sprite->Rotate3(delta);
	next = true;
}

}
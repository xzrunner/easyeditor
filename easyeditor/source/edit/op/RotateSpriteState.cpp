#include "RotateSpriteState.h"
#include "RotateSpriteAOP.h"
#include "Math2D.h"
#include "SpriteSelection.h"
#include "panel_msg.h"

namespace ee
{

RotateSpriteState::RotateSpriteState(SpriteSelection* selection, 
									 const sm::vec2& first_pos)
	: m_angle(0)
{
	m_selection = selection;
	m_selection->AddReference();

	m_first_pos = m_last_pos = first_pos;
}

RotateSpriteState::~RotateSpriteState()
{
	m_selection->RemoveReference();
}

void RotateSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	if (pos != m_first_pos) {
		AtomicOP* aop = new RotateSpriteAOP(*m_selection, m_angle);
		EditAddRecordSJ::Instance()->Add(aop);
	}
}

bool RotateSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	if (m_selection->Size() != 1) return false;

	Rotate(pos);
	m_last_pos = pos;

	return true;
}

void RotateSpriteState::Rotate(const sm::vec2& dst)
{
	m_selection->Traverse(RotateVisitor(m_last_pos, dst, m_angle));
}

//////////////////////////////////////////////////////////////////////////
// class RotateSpriteState::RotateVisitor
//////////////////////////////////////////////////////////////////////////

void RotateSpriteState::RotateVisitor::
Visit(Sprite* spr, bool& next)
{
	sm::vec2 center = spr->GetPosition() + spr->GetOffset();
	float angle = Math2D::GetAngleInDirection(center, m_start, m_end);
	spr->Rotate(angle);

	m_angle += angle;

	next = false;
}

}
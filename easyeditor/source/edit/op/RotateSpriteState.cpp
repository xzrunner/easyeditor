#include "RotateSpriteState.h"
#include "RotateSpriteAOP.h"
#include "Math2D.h"
#include "SpriteSelection.h"
#include "panel_msg.h"

namespace ee
{

RotateSpriteState::RotateSpriteState(SpriteSelection* selection, 
									 const sm::vec2& first_pos)
{
	m_selection = selection;
	m_selection->Retain();

	m_first_pos = m_last_pos = first_pos;
}

RotateSpriteState::~RotateSpriteState()
{
	m_selection->Release();
}

void RotateSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	if (pos != m_first_pos) {
		AtomicOP* aop = new RotateSpriteAOP(*m_selection, m_first_pos, pos);
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
	m_selection->Traverse(RotateVisitor(m_last_pos, dst));
}

//////////////////////////////////////////////////////////////////////////
// class RotateSpriteState::RotateVisitor
//////////////////////////////////////////////////////////////////////////

void RotateSpriteState::RotateVisitor::
Visit(Object* object, bool& next)
{
	Sprite* sprite = static_cast<Sprite*>(object);

	sm::vec2 center = sprite->GetPosition() + sprite->GetOffset();
	float angle = Math2D::GetAngleInDirection(center, m_start, m_end);
	sprite->Rotate(angle);

	next = false;
}

}
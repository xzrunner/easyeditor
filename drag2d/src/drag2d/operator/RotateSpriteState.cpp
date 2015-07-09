#include "RotateSpriteState.h"

#include "history/RotateSpriteAOP.h"
#include "common/Math.h"
#include "view/SpriteSelection.h"

namespace d2d
{

RotateSpriteState::RotateSpriteState(SpriteSelection* selection, 
									 const Vector& first_pos)
{
	m_selection = selection;
	m_selection->Retain();

	m_first_pos = m_last_pos = first_pos;
}

RotateSpriteState::~RotateSpriteState()
{
	m_selection->Release();
}

AbstractAtomicOP* RotateSpriteState::OnMouseRelease(const Vector& pos)
{
	if (pos != m_first_pos) {
		return new RotateSpriteAOP(*m_selection, m_first_pos, pos);
	}
	return NULL;
}

bool RotateSpriteState::OnMouseDrag(const Vector& pos)
{
	if (m_selection->Size() != 1) return false;

	Rotate(pos);
	m_last_pos = pos;

	return true;
}

void RotateSpriteState::Rotate(const Vector& dst)
{
	m_selection->Traverse(Visitor(m_last_pos, dst));
}

//////////////////////////////////////////////////////////////////////////
// class RotateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void RotateSpriteState::Visitor::
	Visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);

	Vector center = sprite->getPosition() + sprite->getOffset();
	float angle = Math::getAngleInDirection(center, m_start, m_end);
	sprite->rotate(angle);

	bFetchNext = false;
}

}
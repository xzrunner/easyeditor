#include "RotateSpriteState.h"

#include "common/Math.h"
#include "history/ArrangeSpriteAtomicOP.h"

namespace d2d
{

RotateSpriteState::RotateSpriteState(SpriteSelection* selection, 
									 const Vector& first_pos)
{
	m_selection = selection;
	m_selection->retain();

	m_first_pos = m_last_pos = first_pos;
}

RotateSpriteState::~RotateSpriteState()
{
	m_selection->release();
}

bool RotateSpriteState::OnMousePress(const Vector& pos)
{
	if (m_selection->size() != 1) return false;

	Rotate(pos);
	m_last_pos = pos;

	return true;
}

AbstractAtomicOP* RotateSpriteState::OnMouseRelease(const Vector& pos)
{
	if (pos != m_first_pos) {
		return new arrange_sprite::RotateSpritesAOP(*m_selection, m_first_pos, pos);
	}
	return NULL;
}

void RotateSpriteState::Rotate(const Vector& dst)
{
	m_selection->traverse(Visitor(m_last_pos, dst));
}

//////////////////////////////////////////////////////////////////////////
// class RotateSpriteState::Visitor
//////////////////////////////////////////////////////////////////////////

void RotateSpriteState::Visitor::
	visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);

	Vector center = sprite->getPosition() + sprite->getOffset();
	float angle = Math::getAngleInDirection(center, m_start, m_end);
	sprite->rotate(angle);

	bFetchNext = false;
}

}
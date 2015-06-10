#include "OffsetSpriteState.h"

#include "common/Matrix.h"
#include "dataset/ISprite.h"
#include "history/OffsetSpriteAOP.h"

namespace d2d
{

OffsetSpriteState::OffsetSpriteState(ISprite* sprite)
{
	m_sprite = sprite;
	m_sprite->Retain();

	m_old_offset = m_sprite->getOffset();
}

OffsetSpriteState::~OffsetSpriteState()
{
	m_sprite->Release();
}

AbstractAtomicOP* OffsetSpriteState::OnMouseRelease(const Vector& pos)
{
	d2d::Vector new_offset = Math::rotateVector(pos - m_sprite->getCenter(), -m_sprite->getAngle());
	return new OffsetSpriteAOP(m_sprite, new_offset, m_old_offset);
}

bool OffsetSpriteState::OnMouseDrag(const Vector& pos)
{
	d2d::Vector offset = Math::rotateVector(pos - m_sprite->getCenter(), -m_sprite->getAngle());
	m_sprite->setOffset(offset);
	return true;
}

}
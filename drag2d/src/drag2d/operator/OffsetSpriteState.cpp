#include "OffsetSpriteState.h"

#include "common/Matrix.h"
#include "dataset/ISprite.h"

namespace d2d
{

OffsetSpriteState::OffsetSpriteState(ISprite* sprite)
{
	m_sprite = sprite;
	m_sprite->retain();
}

OffsetSpriteState::~OffsetSpriteState()
{
	m_sprite->release();
}

bool OffsetSpriteState::OnMousePress(const Vector& pos)
{
	d2d::Vector offset = Math::rotateVector(pos - m_sprite->getCenter(), -m_sprite->getAngle());
	m_sprite->setOffset(offset);
	return true;
}

}
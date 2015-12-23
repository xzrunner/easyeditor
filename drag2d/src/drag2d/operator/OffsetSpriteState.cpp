#include "OffsetSpriteState.h"

#include "common/Matrix.h"
#include "dataset/ISprite.h"
#include "history/OffsetSpriteAOP.h"
#include "message/panel_msg.h"

namespace d2d
{

OffsetSpriteState::OffsetSpriteState(ISprite* sprite)
{
	m_sprite = sprite;
	m_sprite->Retain();

	m_old_offset = m_sprite->GetOffset();
}

OffsetSpriteState::~OffsetSpriteState()
{
	m_sprite->Release();
}

void OffsetSpriteState::OnMouseRelease(const Vector& pos)
{
	Vector new_offset = Math::rotateVector(pos - m_sprite->GetCenter(), -m_sprite->GetAngle());
	AbstractAtomicOP* aop = new OffsetSpriteAOP(m_sprite, new_offset, m_old_offset);
	EditAddRecordSJ::Instance()->Add(aop);
}

bool OffsetSpriteState::OnMouseDrag(const Vector& pos)
{
	Vector offset = Math::rotateVector(pos - m_sprite->GetCenter(), -m_sprite->GetAngle());
	m_sprite->SetOffset(offset);
	return true;
}

}
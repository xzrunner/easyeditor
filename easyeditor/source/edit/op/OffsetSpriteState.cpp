#include "OffsetSpriteState.h"
#include "Sprite.h"
#include "OffsetSpriteAOP.h"
#include "panel_msg.h"
#include "Math2D.h"

namespace ee
{

OffsetSpriteState::OffsetSpriteState(Sprite* sprite)
{
	m_sprite = sprite;
	m_sprite->Retain();

	m_old_offset = m_sprite->GetOffset();
}

OffsetSpriteState::~OffsetSpriteState()
{
	m_sprite->Release();
}

void OffsetSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	sm::vec2 new_offset = Math2D::RotateVector(pos - m_sprite->GetCenter(), -m_sprite->GetAngle());
	AtomicOP* aop = new OffsetSpriteAOP(m_sprite, new_offset, m_old_offset);
	EditAddRecordSJ::Instance()->Add(aop);
}

bool OffsetSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	sm::vec2 offset = Math2D::RotateVector(pos - m_sprite->GetCenter(), -m_sprite->GetAngle());
	m_sprite->SetOffset(offset);
	return true;
}

}
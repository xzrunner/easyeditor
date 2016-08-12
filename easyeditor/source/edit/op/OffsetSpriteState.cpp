#include "OffsetSpriteState.h"
#include "Sprite.h"
#include "OffsetSpriteAOP.h"
#include "panel_msg.h"
#include "Math2D.h"
#include "SpriteCtrlNode.h"
#include "ArrangeSpriteImpl.h"

#include <SM_Calc.h>

namespace ee
{

OffsetSpriteState::OffsetSpriteState(Sprite* sprite)
{
	m_sprite = sprite;
	m_sprite->AddReference();

	m_old_offset = m_sprite->GetOffset();
}

OffsetSpriteState::~OffsetSpriteState()
{
	m_sprite->RemoveReference();
}

void OffsetSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	sm::vec2 ctrl_nodes[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(m_sprite, ctrl_nodes);
	sm::vec2 fixed = pos;
	for (int i = 0; i < 8; ++i) {
		if (sm::dis_pos_to_pos(fixed, ctrl_nodes[i]) < ArrangeSpriteImpl::CTRL_NODE_RADIUS) {
			fixed = ctrl_nodes[i];
			break;
		}
	}

	sm::vec2 new_offset = Math2D::RotateVector(fixed - m_sprite->GetCenter(), -m_sprite->GetAngle());
	m_sprite->SetOffset(new_offset);
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
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

OffsetSpriteState::OffsetSpriteState(Sprite* spr)
{
	m_spr = spr;
	m_spr->AddReference();

	m_old_offset = m_spr->GetOffset();
}

OffsetSpriteState::~OffsetSpriteState()
{
	m_spr->RemoveReference();
}

void OffsetSpriteState::OnMouseRelease(const sm::vec2& pos)
{
	sm::vec2 ctrl_nodes[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(m_spr, ctrl_nodes);
	sm::vec2 fixed = pos;
	for (int i = 0; i < 8; ++i) {
		if (sm::dis_pos_to_pos(fixed, ctrl_nodes[i]) < ArrangeSpriteImpl::CTRL_NODE_RADIUS) {
			fixed = ctrl_nodes[i];
			break;
		}
	}

	sm::vec2 new_offset = Math2D::RotateVector(fixed - m_spr->GetCenter(), -m_spr->GetAngle());
	m_spr->SetOffset(new_offset);
	AtomicOP* aop = new OffsetSpriteAOP(m_spr, new_offset, m_old_offset);
	EditAddRecordSJ::Instance()->Add(aop);
}

bool OffsetSpriteState::OnMouseDrag(const sm::vec2& pos)
{
	sm::vec2 offset = Math2D::RotateVector(pos - m_spr->GetCenter(), -m_spr->GetAngle());
	m_spr->SetOffset(offset);
	return true;
}

}